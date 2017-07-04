/* Copyright (C) 2007-2010 Open Information Security Foundation
 *
 * You can copy, redistribute or modify this Program under the terms of
 * the GNU General Public License version 2 as published by the Free
 * Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * \file
 *
 * \author Endace Technology Limited - Jason Ish <jason.ish@endace.com>
 *
 * This file provides a basic configuration system for the IDPS
 * engine.
 *
 * NOTE: Setting values should only be done from one thread during
 * engine initialization.  Multiple threads should be able access read
 * configuration data.  Allowing run time changes to the configuration
 * will require some locks.
 *
 * \todo Consider having the in-memory configuration database a direct
 *   reflection of the configuration file and moving command line
 *   parameters to a primary lookup table?
 *
 * \todo Get rid of allow override and go with a simpler first set,
 *   stays approach?
 */

#include "conf.h"

/** Maximum size of a complete domain name. */
#define NODE_NAME_MAX 1024

static ConfNode *root = NULL;
static ConfNode *root_backup = NULL;

/**
 * \brief Helper function to get a node, creating it if it does not
 * exist.
 *
 * This function exits on memory failure as creating configuration
 * nodes is usually part of application initialization.
 *
 * \param name The name of the configuration node to get.
 * \param final Flag to set created nodes as final or not.
 *
 * \retval The existing configuration node if it exists, or a newly
 *   created node for the provided name.  On error, NULL will be returned.
 */
static ConfNode *
ConfGetNodeOrCreate(char *name, int final)
{
    ConfNode *parent = root;
    ConfNode *node = NULL;
    char node_name[NODE_NAME_MAX];
    char *key;
    char *next;

    if (strlcpy(node_name, name, sizeof(node_name)) >= sizeof(node_name)) {
        printf(
            "Configuration name too long: %s", name);
        return NULL;
    }

    key = node_name;

    do {
        if ((next = strchr(key, '.')) != NULL)
            *next++ = '\0';
        if ((node = ConfNodeLookupChild(parent, key)) == NULL) {
            node = ConfNodeNew();
            if (unlikely(node == NULL)) {
                printf(
                    "Failed to allocate memory for configuration.");
                goto end;
            }
            node->name = strdup(key);
            if (unlikely(node->name == NULL)) {
                ConfNodeFree(node);
                node = NULL;
                printf(
                    "Failed to allocate memory for configuration.");
                goto end;
            }
            node->parent = parent;
            node->final = final;
            TAILQ_INSERT_TAIL(&parent->head, node, next);
        }
        key = next;
        parent = node;
    } while (next != NULL);

end:
    return node;
}

/**
 * \brief Initialize the configuration system.
 */
void
ConfInit(void)
{
    if (root != NULL) {
        printf("already initialized\n");
        return;
    }
    root = ConfNodeNew();
    if (root == NULL) {
        printf(
            "ERROR: Failed to allocate memory for root configuration node, "
            "aborting.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * \brief Allocate a new configuration node.
 *
 * \retval An allocated configuration node on success, NULL on failure.
 */
ConfNode *
ConfNodeNew(void)
{
    ConfNode *new;

    new = calloc(1, sizeof(*new));
    if (unlikely(new == NULL)) {
        return NULL;
    }
    TAILQ_INIT(&new->head);

    return new;
}

/**
 * \brief Free a ConfNode and all of its children.
 *
 * \param node The configuration node to free.
 */
void
ConfNodeFree(ConfNode *node)
{
    ConfNode *tmp;

    while ((tmp = TAILQ_FIRST(&node->head))) {
        TAILQ_REMOVE(&node->head, tmp, next);
        ConfNodeFree(tmp);
    }

    if (node->name != NULL)
        free(node->name);
    if (node->val != NULL)
        free(node->val);
    free(node);
}

/**
 * \brief Get a ConfNode by name.
 *
 * \param name The full name of the configuration node to lookup.
 *
 * \retval A pointer to ConfNode is found or NULL if the configuration
 *    node does not exist.
 */
ConfNode *
ConfGetNode(char *name)
{
    ConfNode *node = root;
    char node_name[NODE_NAME_MAX];
    char *key;
    char *next;

    if (strlcpy(node_name, name, sizeof(node_name)) >= sizeof(node_name)) {
        printf(
            "Configuration name too long: %s\n", name);
        return NULL;
    }

    key = node_name;
    do {
        if ((next = strchr(key, '.')) != NULL)
            *next++ = '\0';
        node = ConfNodeLookupChild(node, key);
        key = next;
    } while (next != NULL && node != NULL);

    return node;
}

/**
 * \brief Get the root configuration node.
 */
ConfNode *
ConfGetRootNode(void)
{
    return root;
}

/**
 * \brief Set a configuration value.
 *
 * Configuration values set with this function may be overridden by
 * subsequent calls, or if the value appears multiple times in a
 * configuration file.
 *
 * \param name The name of the configuration parameter to set.
 * \param val The value of the configuration parameter.
 *
 * \retval 1 if the value was set otherwise 0.
 */
int
ConfSet(char *name, char *val)
{
    ConfNode *node = ConfGetNodeOrCreate(name, 0);
    if (node == NULL || node->final) {
        return 0;
    }
    if (node->val != NULL)
        free(node->val);
    node->val = strdup(val);
    if (unlikely(node->val == NULL)) {
        return 0;
    }
    return 1;
}

/**
 * \brief Set a final configuration value.
 *
 * A final configuration value is a value that cannot be overridden by
 * the configuration file.  Its mainly useful for setting values that
 * are supplied on the command line prior to the configuration file
 * being loaded.  However, a subsequent call to this function can
 * override a previously set value.
 *
 * \param name The name of the configuration parameter to set.
 * \param val The value of the configuration parameter.
 *
 * \retval 1 if the value was set otherwise 0.
 */
int
ConfSetFinal(char *name, char *val)
{
    ConfNode *node = ConfGetNodeOrCreate(name, 1);
    if (node == NULL) {
        return 0;
    }
    if (node->val != NULL)
        free(node->val);
    node->val = strdup(val);
    if (unlikely(node->val == NULL)) {
        return 0;
    }
    node->final = 1;
    return 1;
}

/**
 * \brief Retrieve the value of a configuration node.
 *
 * This function will return the value for a configuration node based
 * on the full name of the node.  It is possible that the value
 * returned could be NULL, this could happen if the requested node
 * does exist but is not a node that contains a value, but contains
 * children ConfNodes instead.
 *
 * \param name Name of configuration parameter to get.
 * \param vptr Pointer that will be set to the configuration value parameter.
 *   Note that this is just a reference to the actual value, not a copy.
 *
 * \retval 1 will be returned if the name is found, otherwise 0 will
 *   be returned.
 */
int
ConfGet(char *name, char **vptr)
{
    ConfNode *node = ConfGetNode(name);
    if (node == NULL) {
        printf("failed to lookup configuration parameter '%s'\n", name);
        return 0;
    }
    else {
        *vptr = node->val;
        return 1;
    }
}

int ConfGetChildValue(ConfNode *base, char *name, char **vptr)
{
    ConfNode *node = ConfNodeLookupChild(base, name);

    if (node == NULL) {
        printf("failed to lookup configuration parameter '%s'\n", name);
        return 0;
    }
    else {
        *vptr = node->val;
        return 1;
    }
}


int ConfGetChildValueWithDefault(ConfNode *base, ConfNode *dflt, char *name, char **vptr)
{
    int ret = ConfGetChildValue(base, name, vptr);
    /* Get 'default' value */
    if (ret == 0 && dflt) {
        return ConfGetChildValue(dflt, name, vptr);
    }
    return ret;
}

/**
 * \brief Retrieve a configuration value as an integer.
 *
 * \param name Name of configuration parameter to get.
 * \param val Pointer to an intmax_t that will be set the
 * configuration value.
 *
 * \retval 1 will be returned if the name is found and was properly
 * converted to an interger, otherwise 0 will be returned.
 */
int
ConfGetInt(char *name, intmax_t *val)
{
    char *strval;
    intmax_t tmpint;
    char *endptr;

    if (ConfGet(name, &strval) == 0)
        return 0;

    errno = 0;
    tmpint = strtoimax(strval, &endptr, 0);
    if (strval[0] == '\0' || *endptr != '\0')
        return 0;
    if (errno == ERANGE && (tmpint == INTMAX_MAX || tmpint == INTMAX_MIN))
        return 0;

    *val = tmpint;
    return 1;
}

int ConfGetChildValueInt(ConfNode *base, char *name, intmax_t *val)
{
    char *strval;
    intmax_t tmpint;
    char *endptr;

    if (ConfGetChildValue(base, name, &strval) == 0)
        return 0;
    errno = 0;
    tmpint = strtoimax(strval, &endptr, 0);
    if (strval[0] == '\0' || *endptr != '\0')
        return 0;
    if (errno == ERANGE && (tmpint == INTMAX_MAX || tmpint == INTMAX_MIN))
        return 0;

    *val = tmpint;
    return 1;

}

int ConfGetChildValueIntWithDefault(ConfNode *base, ConfNode *dflt, char *name, intmax_t *val)
{
    int ret = ConfGetChildValueInt(base, name, val);
    /* Get 'default' value */
    if (ret == 0 && dflt) {
        return ConfGetChildValueInt(dflt, name, val);
    }
    return ret;
}


/**
 * \brief Retrieve a configuration value as an boolen.
 *
 * \param name Name of configuration parameter to get.
 * \param val Pointer to an int that will be set to 1 for true, or 0
 * for false.
 *
 * \retval 1 will be returned if the name is found and was properly
 * converted to a boolean, otherwise 0 will be returned.
 */
int
ConfGetBool(char *name, int *val)
{
    char *strval;

    *val = 0;
    if (ConfGet(name, &strval) != 1)
        return 0;

    *val = ConfValIsTrue(strval);

    return 1;
}

int ConfGetChildValueBool(ConfNode *base, char *name, int *val)
{
    char *strval;

    *val = 0;
    if (ConfGetChildValue(base, name, &strval) == 0)
        return 0;

    *val = ConfValIsTrue(strval);

    return 1;
}

int ConfGetChildValueBoolWithDefault(ConfNode *base, ConfNode *dflt, char *name, int *val)
{
    int ret = ConfGetChildValueBool(base, name, val);
    /* Get 'default' value */
    if (ret == 0 && dflt) {
        return ConfGetChildValueBool(dflt, name, val);
    }
    return ret;
}


/**
 * \brief Check if a value is true.
 *
 * The value is considered true if it is a string with the value of 1,
 * yes, true or on.  The test is not case sensitive, any other value
 * is false.
 *
 * \param val The string to test for a true value.
 *
 * \retval 1 If the value is true, 0 if not.
 */
int
ConfValIsTrue(const char *val)
{
    char *trues[] = {"1", "yes", "true", "on"};
    size_t u;

    for (u = 0; u < sizeof(trues) / sizeof(trues[0]); u++) {
        if (strcasecmp(val, trues[u]) == 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * \brief Check if a value is false.
 *
 * The value is considered false if it is a string with the value of 0,
 * no, false or off.  The test is not case sensitive, any other value
 * is not false.
 *
 * \param val The string to test for a false value.
 *
 * \retval 1 If the value is false, 0 if not.
 */
int
ConfValIsFalse(const char *val)
{
    char *falses[] = {"0", "no", "false", "off"};
    size_t u;

    for (u = 0; u < sizeof(falses) / sizeof(falses[0]); u++) {
        if (strcasecmp(val, falses[u]) == 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * \brief Retrieve a configuration value as a double
 *
 * \param name Name of configuration parameter to get.
 * \param val Pointer to an double that will be set the
 * configuration value.
 *
 * \retval 1 will be returned if the name is found and was properly
 * converted to a double, otherwise 0 will be returned.
 */
int
ConfGetDouble(char *name, double *val)
{
    char *strval;
    double tmpdo;
    char *endptr;

    if (ConfGet(name, &strval) == 0)
        return 0;

    errno = 0;
    tmpdo = strtod(strval, &endptr);
    if (strval[0] == '\0' || *endptr != '\0')
        return 0;
    if (errno == ERANGE)
        return 0;

    *val = tmpdo;
    return 1;
}

/**
 * \brief Retrieve a configuration value as a float
 *
 * \param name Name of configuration parameter to get.
 * \param val Pointer to an float that will be set the
 * configuration value.
 *
 * \retval 1 will be returned if the name is found and was properly
 * converted to a double, otherwise 0 will be returned.
 */
int
ConfGetFloat(char *name, float *val)
{
    char *strval;
    double tmpfl;
    char *endptr;

    if (ConfGet(name, &strval) == 0)
        return 0;

    errno = 0;
    tmpfl = strtof(strval, &endptr);
    if (strval[0] == '\0' || *endptr != '\0')
        return 0;
    if (errno == ERANGE)
        return 0;

    *val = tmpfl;
    return 1;
}

/**
 * \brief Remove (and free) the provided configuration node.
 */
void
ConfNodeRemove(ConfNode *node)
{
    if (node->parent != NULL)
        TAILQ_REMOVE(&node->parent->head, node, next);
    ConfNodeFree(node);
}

/**
 * \brief Remove a configuration parameter from the configuration db.
 *
 * \param name The name of the configuration parameter to remove.
 *
 * \retval Returns 1 if the parameter was removed, otherwise 0 is returned
 *   most likely indicating the parameter was not set.
 */
int
ConfRemove(char *name)
{
    ConfNode *node;

    node = ConfGetNode(name);
    if (node == NULL)
        return 0;
    else {
        ConfNodeRemove(node);
        return 1;
    }
}

/**
 * \brief Creates a backup of the conf_hash hash_table used by the conf API.
 */
void
ConfCreateContextBackup(void)
{
    root_backup = root;
    root = NULL;

    return;
}

/**
 * \brief Restores the backup of the hash_table present in backup_conf_hash
 *        back to conf_hash.
 */
void
ConfRestoreContextBackup(void)
{
    root = root_backup;
    root_backup = NULL;

    return;
}

/**
 * \brief De-initializes the configuration system.
 */
void
ConfDeInit(void)
{
    if (root != NULL) {
        ConfNodeFree(root);
        root = NULL;
    }

    printf("configuration module de-initialized\n");
}

static char *
ConfPrintNameArray(char **name_arr, int level)
{
    static char name[128*128];
    int i;

    name[0] = '\0';
    for (i = 0; i <= level; i++) {
        strlcat(name, name_arr[i], sizeof(name));
        if (i < level)
            strlcat(name, ".", sizeof(name));
    }

    return name;
}

/**
 * \brief Dump a configuration node and all its children.
 */
void
ConfNodeDump(ConfNode *node, const char *prefix)
{
    ConfNode *child;

    static char *name[128];
    static int level = -1;

    level++;
    TAILQ_FOREACH(child, &node->head, next) {
        name[level] = strdup(child->name);
        if (unlikely(name[level] == NULL)) {
            continue;
        }
        if (prefix == NULL) {
            printf("%s = %s\n", ConfPrintNameArray(name, level),
                child->val);
        }
        else {
            printf("%s.%s = %s\n", prefix,
                ConfPrintNameArray(name, level), child->val);
        }
        ConfNodeDump(child, prefix);
        free(name[level]);
    }
    level--;
}

/**
 * \brief Dump configuration to stdout.
 */
void
ConfDump(void)
{
    ConfNodeDump(root, NULL);
}

/**
 * \brief Lookup a child configuration node by name.
 *
 * Given a ConfNode this function will lookup an immediate child
 * ConfNode by name and return the child ConfNode.
 *
 * \param node The parent configuration node.
 * \param name The name of the child node to lookup.
 *
 * \retval A pointer the child ConfNode if found otherwise NULL.
 */
ConfNode *
ConfNodeLookupChild(ConfNode *node, const char *name)
{
    ConfNode *child;

    TAILQ_FOREACH(child, &node->head, next) {
        if (strcmp(child->name, name) == 0)
            return child;
    }

    return NULL;
}

/**
 * \brief Lookup the value of a child configuration node by name.
 *
 * Given a parent ConfNode this function will return the value of a
 * child configuration node by name returning a reference to that
 * value.
 *
 * \param node The parent configuration node.
 * \param name The name of the child node to lookup.
 *
 * \retval A pointer the child ConfNodes value if found otherwise NULL.
 */
const char *
ConfNodeLookupChildValue(ConfNode *node, const char *name)
{
    ConfNode *child;

    child = ConfNodeLookupChild(node, name);
    if (child != NULL)
        return child->val;

    return NULL;
}

/**
 * \brief Lookup for a key value under a specific node
 *
 * \return the ConfNode matching or NULL
 */

ConfNode *ConfNodeLookupKeyValue(ConfNode *base, const char *key, const char *value)
{
    ConfNode *child;

    TAILQ_FOREACH(child, &base->head, next) {
        if (!strncmp(child->val, key, strlen(child->val))) {
            ConfNode *subchild;
            TAILQ_FOREACH(subchild, &child->head, next) {
                if ((!strcmp(subchild->name, key)) && (!strcmp(subchild->val, value))) {
                    return child;
                }
            }
        }
    }

    return NULL;
}

/**
 * \brief Test if a configuration node has a true value.
 *
 * \param node The parent configuration node.
 * \param name The name of the child node to test.
 *
 * \retval 1 if the child node has a true value, otherwise 0 is
 *     returned, even if the child node does not exist.
 */
int
ConfNodeChildValueIsTrue(ConfNode *node, const char *key)
{
    const char *val;

    val = ConfNodeLookupChildValue(node, key);

    return val != NULL ? ConfValIsTrue(val) : 0;
}

/**
 *  \brief Create the path for an include entry
 *  \param file The name of the file
 *  \retval str Pointer to the string path + sig_file
 */
char *ConfLoadCompleteIncludePath(char *file)
{
    char *defaultpath = NULL;
    char *path = NULL;

    /* Path not specified */
    if (PathIsRelative(file)) {
        if (ConfGet("include-path", &defaultpath) == 1) {
            printf("Default path: %s", defaultpath);
            size_t path_len = sizeof(char) * (strlen(defaultpath) +
                          strlen(file) + 2);
            path = malloc(path_len);
            if (unlikely(path == NULL))
                return NULL;
            strlcpy(path, defaultpath, path_len);
            if (path[strlen(path) - 1] != '/')
                strlcat(path, "/", path_len);
            strlcat(path, file, path_len);
       } else {
            path = strdup(file);
            if (unlikely(path == NULL))
                return NULL;
        }
    } else {
        path = strdup(file);
        if (unlikely(path == NULL))
            return NULL;
    }
    return path;
}

/**
 * \brief Prune a configuration node.
 *
 * Pruning a configuration is similar to freeing, but only fields that
 * may be overridden are, leaving final type parameters.  Additional
 * the value of the provided node is also free'd, but the node itself
 * is left.
 *
 * \param node The configuration node to prune.
 */
void
ConfNodePrune(ConfNode *node)
{
    ConfNode *item, *it;

    for (item = TAILQ_FIRST(&node->head); item != NULL; item = it) {
        it = TAILQ_NEXT(item, next);
        if (!item->final) {
            ConfNodePrune(item);
            if (TAILQ_EMPTY(&item->head)) {
                TAILQ_REMOVE(&node->head, item, next);
                if (item->name != NULL)
                    free(item->name);
                if (item->val != NULL)
                    free(item->val);
                free(item);
            }
        }
    }

    if (node->val != NULL) {
        free(node->val);
        node->val = NULL;
    }
}

#ifdef UNITTESTS

/**
 * Lookup a non-existant value.
 */
static int
ConfTestGetNonExistant(void)
{
    char name[] = "non-existant-value";
    char *value;

    return !ConfGet(name, &value);
}

/**
 * Set then lookup a value.
 */
static int
ConfTestSetAndGet(void)
{
    char name[] = "some-name";
    char value[] = "some-value";
    char *value0;

    if (ConfSet(name, value) != 1)
        return 0;
    if (ConfGet(name, &value0) != 1)
        return 0;
    if (strcmp(value, value0) != 0)
        return 0;

    /* Cleanup. */
    ConfRemove(name);

    return 1;
}

/**
 * Test that overriding a value is allowed provided allow_override is
 * true and that the config parameter gets the new value.
 */
static int
ConfTestOverrideValue1(void)
{
    char name[] = "some-name";
    char value0[] = "some-value";
    char value1[] = "new-value";
    char *val;
    int rc;

    if (ConfSet(name, value0) != 1)
        return 0;
    if (ConfSet(name, value1) != 1)
        return 0;
    if (ConfGet(name, &val) != 1)
        return 0;

    rc = !strcmp(val, value1);

    /* Cleanup. */
    ConfRemove(name);

    return rc;
}

/**
 * Test that a final value will not be overrided by a ConfSet.
 */
static int
ConfTestOverrideValue2(void)
{
    char name[] = "some-name";
    char value0[] = "some-value";
    char value1[] = "new-value";
    char *val;
    int rc;

    if (ConfSetFinal(name, value0) != 1)
        return 0;
    if (ConfSet(name, value1) != 0)
        return 0;
    if (ConfGet(name, &val) != 1)
        return 0;

    rc = !strcmp(val, value0);

    /* Cleanup. */
    ConfRemove(name);

    return rc;
}

/**
 * Test retrieving an integer value from the configuration db.
 */
static int
ConfTestGetInt(void)
{
    char name[] = "some-int.x";
    intmax_t val;

    if (ConfSet(name, "0") != 1)
        return 0;
    if (ConfGetInt(name, &val) != 1)
        return 0;

    if (val != 0)
        return 0;

    if (ConfSet(name, "-1") != 1)
        return 0;
    if (ConfGetInt(name, &val) != 1)
        return 0;
    if (val != -1)
        return 0;

    if (ConfSet(name, "0xffff") != 1)
        return 0;
    if (ConfGetInt(name, &val) != 1)
        return 0;
    if (val != 0xffff)
        return 0;

    if (ConfSet(name, "not-an-int") != 1)
        return 0;
    if (ConfGetInt(name, &val) != 0)
        return 0;

    return 1;
}

/**
 * Test retrieving a boolean value from the configuration db.
 */
static int
ConfTestGetBool(void)
{
    char name[] = "some-bool";
    char *trues[] = {
        "1",
        "on", "ON",
        "yes", "YeS",
        "true", "TRUE",
    };
    char *falses[] = {
        "0",
        "something",
        "off", "OFF",
        "false", "FalSE",
        "no", "NO",
    };
    int val;
    size_t u;

    for (u = 0; u < sizeof(trues) / sizeof(trues[0]); u++) {
        if (ConfSet(name, trues[u]) != 1)
            return 0;
        if (ConfGetBool(name, &val) != 1)
            return 0;
        if (val != 1)
            return 0;
    }

    for (u = 0; u < sizeof(falses) / sizeof(falses[0]); u++) {
        if (ConfSet(name, falses[u]) != 1)
            return 0;
        if (ConfGetBool(name, &val) != 1)
            return 0;
        if (val != 0)
            return 0;
    }

    return 1;
}

static int
ConfNodeLookupChildTest(void)
{
    char *test_vals[] = { "one", "two", "three" };
    size_t u;

    ConfNode *parent = ConfNodeNew();
    ConfNode *child;

    for (u = 0; u < sizeof(test_vals)/sizeof(test_vals[0]); u++) {
        child = ConfNodeNew();
        child->name = strdup(test_vals[u]);
        child->val = strdup(test_vals[u]);
        TAILQ_INSERT_TAIL(&parent->head, child, next);
    }

    child = ConfNodeLookupChild(parent, "one");
    if (child == NULL)
        return 0;
    if (strcmp(child->name, "one") != 0)
        return 0;
    if (strcmp(child->val, "one") != 0)
        return 0;

    child = ConfNodeLookupChild(parent, "two");
    if (child == NULL)
        return 0;
    if (strcmp(child->name, "two") != 0)
        return 0;
    if (strcmp(child->val, "two") != 0)
        return 0;

    child = ConfNodeLookupChild(parent, "three");
    if (child == NULL)
        return 0;
    if (strcmp(child->name, "three") != 0)
        return 0;
    if (strcmp(child->val, "three") != 0)
        return 0;

    child = ConfNodeLookupChild(parent, "four");
    if (child != NULL)
        return 0;

    ConfNodeFree(parent);

    return 1;
}

static int
ConfNodeLookupChildValueTest(void)
{
    char *test_vals[] = { "one", "two", "three" };
    size_t u;

    ConfNode *parent = ConfNodeNew();
    ConfNode *child;
    const char *value;

    for (u = 0; u < sizeof(test_vals)/sizeof(test_vals[0]); u++) {
        child = ConfNodeNew();
        child->name = strdup(test_vals[u]);
        child->val = strdup(test_vals[u]);
        TAILQ_INSERT_TAIL(&parent->head, child, next);
    }

    value = (char *)ConfNodeLookupChildValue(parent, "one");
    if (value == NULL)
        return 0;
    if (strcmp(value, "one") != 0)
        return 0;

    value = (char *)ConfNodeLookupChildValue(parent, "two");
    if (value == NULL)
        return 0;
    if (strcmp(value, "two") != 0)
        return 0;

    value = (char *)ConfNodeLookupChildValue(parent, "three");
    if (value == NULL)
        return 0;
    if (strcmp(value, "three") != 0)
        return 0;

    value = (char *)ConfNodeLookupChildValue(parent, "four");
    if (value != NULL)
        return 0;

    ConfNodeFree(parent);

    return 1;
}

static int ConfGetChildValueWithDefaultTest(void)
{
    char  *val = "";
    int ret = 1;
    ConfCreateContextBackup();
    ConfInit();
    ConfSet("af-packet.0.interface", "eth0");
    ConfSet("af-packet.1.interface", "default");
    ConfSet("af-packet.1.cluster-type", "cluster_cpu");

    ConfNode *root = ConfGetNode("af-packet.0");
    ConfNode *dflt = ConfGetNode("af-packet.1");
    ConfGetChildValueWithDefault(root, dflt, "cluster-type", &val);
    if (strcmp(val, "cluster_cpu")) {
        ConfDeInit();
        ConfRestoreContextBackup();
        return 0;
    }

    ConfSet("af-packet.0.cluster-type", "cluster_flow");
    ConfGetChildValueWithDefault(root, dflt, "cluster-type", &val);

    if (strcmp(val, "cluster_flow")) {
        ret = 0;
    }
    ConfDeInit();
    ConfRestoreContextBackup();
    return ret;
}

static int ConfGetChildValueIntWithDefaultTest(void)
{
    intmax_t val = 0;
    ConfCreateContextBackup();
    ConfInit();
    ConfSet("af-packet.0.interface", "eth0");
    ConfSet("af-packet.1.interface", "default");
    ConfSet("af-packet.1.threads", "2");

    ConfNode *root = ConfGetNode("af-packet.0");
    ConfNode *dflt = ConfGetNode("af-packet.1");
    ConfGetChildValueIntWithDefault(root, dflt, "threads", &val);
    if (val != 2) {
        ConfDeInit();
        ConfRestoreContextBackup();
        return 0;
    }

    ConfSet("af-packet.0.threads", "1");
    ConfGetChildValueIntWithDefault(root, dflt, "threads", &val);

    ConfDeInit();
    ConfRestoreContextBackup();
    if (val != 1) {
        return 0;
    }
    return 1;
}

static int ConfGetChildValueBoolWithDefaultTest(void)
{
    int val;
    ConfCreateContextBackup();
    ConfInit();
    ConfSet("af-packet.0.interface", "eth0");
    ConfSet("af-packet.1.interface", "default");
    ConfSet("af-packet.1.use-mmap", "yes");

    ConfNode *root = ConfGetNode("af-packet.0");
    ConfNode *dflt = ConfGetNode("af-packet.1");
    ConfGetChildValueBoolWithDefault(root, dflt, "use-mmap", &val);
    if (val == 0) {
        ConfDeInit();
        ConfRestoreContextBackup();
        return 0;
    }

    ConfSet("af-packet.0.use-mmap", "no");
    ConfGetChildValueBoolWithDefault(root, dflt, "use-mmap", &val);

    ConfDeInit();
    ConfRestoreContextBackup();
    if (val) {
        return 0;
    }
    return 1;
}

/**
 * Test the removal of a configuration node.
 */
static int
ConfNodeRemoveTest(void)
{
    ConfCreateContextBackup();
    ConfInit();

    if (ConfSet("some.nested.parameter", "blah") != 1)
        return 0;

    ConfNode *node = ConfGetNode("some.nested.parameter");
    if (node == NULL)
        return 0;
    ConfNodeRemove(node);

    node = ConfGetNode("some.nested.parameter");
    if (node != NULL)
        return 0;

    ConfDeInit();
    ConfRestoreContextBackup();

    return 1;
}

static int
ConfSetTest(void)
{
    ConfCreateContextBackup();
    ConfInit();

    /* Set some value with 2 levels. */
    if (ConfSet("one.two", "three") != 1)
        return 0;
    ConfNode *n = ConfGetNode("one.two");
    if (n == NULL)
        return 0;

    /* Set another 2 level parameter with the same first level, this
     * used to trigger a bug that caused the second level of the name
     * to become a first level node. */
    if (ConfSet("one.three", "four") != 1)
        return 0;

    n = ConfGetNode("one.three");
    if (n == NULL)
        return 0;

    /* A top level node of "three" should not exist. */
    n = ConfGetNode("three");
    if (n != NULL)
        return 0;

    ConfDeInit();
    ConfRestoreContextBackup();

    return 1;
}

static int
ConfGetNodeOrCreateTest(void)
{
    ConfNode *node;
    int ret = 0;

    ConfCreateContextBackup();
    ConfInit();

    /* Get a node that should not exist, give it a value, re-get it
     * and make sure the second time it returns the existing node. */
    node = ConfGetNodeOrCreate("node0", 0);
    if (node == NULL) {
        fprintf(stderr, "returned null\n");
        goto end;
    }
    if (node->parent == NULL || node->parent != root) {
        fprintf(stderr, "unexpected parent node\n");
        goto end;
    }
    if (node->val != NULL) {
        fprintf(stderr, "node already existed\n");
        goto end;
    }
    node->val = strdup("node0");
    node = ConfGetNodeOrCreate("node0", 0);
    if (node == NULL) {
        fprintf(stderr, "returned null\n");
        goto end;
    }
    if (node->val == NULL) {
        fprintf(stderr, "new node was allocated\n");
        goto end;
    }
    if (strcmp(node->val, "node0") != 0) {
        fprintf(stderr, "node did not have expected value\n");
        goto end;
    }

    /* Do the same, but for something deeply nested. */
    node = ConfGetNodeOrCreate("parent.child.grandchild", 0);
    if (node == NULL) {
        fprintf(stderr, "returned null\n");
        goto end;
    }
    if (node->parent == NULL || node->parent == root) {
        fprintf(stderr, "unexpected parent node\n");
        goto end;
    }
    if (node->val != NULL) {
        fprintf(stderr, "node already existed\n");
        goto end;
    }
    node->val = strdup("parent.child.grandchild");
    node = ConfGetNodeOrCreate("parent.child.grandchild", 0);
    if (node == NULL) {
        fprintf(stderr, "returned null\n");
        goto end;
    }
    if (node->val == NULL) {
        fprintf(stderr, "new node was allocated\n");
        goto end;
    }
    if (strcmp(node->val, "parent.child.grandchild") != 0) {
        fprintf(stderr, "node did not have expected value\n");
        goto end;
    }

    /* Test that 2 child nodes have the same root. */
    ConfNode *child1 = ConfGetNodeOrCreate("parent.kids.child1", 0);
    ConfNode *child2 = ConfGetNodeOrCreate("parent.kids.child2", 0);
    if (child1 == NULL || child2 == NULL) {
        fprintf(stderr, "returned null\n");
        goto end;
    }
    if (child1->parent != child2->parent) {
        fprintf(stderr, "child nodes have different parents\n");
        goto end;
    }
    if (strcmp(child1->parent->name, "kids") != 0) {
        fprintf(stderr, "parent node had unexpected name\n");
        goto end;
    }

    ret = 1;

end:
    ConfDeInit();
    ConfRestoreContextBackup();

    return ret;
}

static int
ConfNodePruneTest(void)
{
    int ret = 0;
    ConfNode *node;

    ConfCreateContextBackup();
    ConfInit();

    /* Test that final nodes exist after a prune. */
    if (ConfSet("node.notfinal", "notfinal") != 1)
        goto end;
    if (ConfSetFinal("node.final", "final") != 1)
        goto end;
    if (ConfGetNode("node.notfinal") == NULL)
        goto end;
    if (ConfGetNode("node.final") == NULL)
        goto end;
    if ((node = ConfGetNode("node")) == NULL)
        goto end;
    ConfNodePrune(node);
    if (ConfGetNode("node.notfinal") != NULL)
        goto end;
    if (ConfGetNode("node.final") == NULL)
        goto end;

    /* Test that everything under a final node exists after a prune. */
    if (ConfSet("node.final.one", "one") != 1)
        goto end;
    if (ConfSet("node.final.two", "two") != 1)
        goto end;
    ConfNodePrune(node);
    if (ConfNodeLookupChild(node, "final") == NULL)
        goto end;
    if (ConfGetNode("node.final.one") == NULL)
        goto end;
    if (ConfGetNode("node.final.two") == NULL)
        goto end;

    ret = 1;

end:
    ConfDeInit();
    ConfRestoreContextBackup();

    return ret;
}

void
ConfRegisterTests(void)
{
    UtRegisterTest("ConfTestGetNonExistant", ConfTestGetNonExistant, 1);
    UtRegisterTest("ConfSetTest", ConfSetTest, 1);
    UtRegisterTest("ConfTestSetAndGet", ConfTestSetAndGet, 1);
    UtRegisterTest("ConfTestOverrideValue1", ConfTestOverrideValue1, 1);
    UtRegisterTest("ConfTestOverrideValue2", ConfTestOverrideValue2, 1);
    UtRegisterTest("ConfTestGetInt", ConfTestGetInt, 1);
    UtRegisterTest("ConfTestGetBool", ConfTestGetBool, 1);
    UtRegisterTest("ConfNodeLookupChildTest", ConfNodeLookupChildTest, 1);
    UtRegisterTest("ConfNodeLookupChildValueTest", ConfNodeLookupChildValueTest, 1);
    UtRegisterTest("ConfNodeRemoveTest", ConfNodeRemoveTest, 1);
    UtRegisterTest("ConfGetChildValueWithDefaultTest", ConfGetChildValueWithDefaultTest, 1);
    UtRegisterTest("ConfGetChildValueIntWithDefaultTest", ConfGetChildValueIntWithDefaultTest, 1);
    UtRegisterTest("ConfGetChildValueBoolWithDefaultTest", ConfGetChildValueBoolWithDefaultTest, 1);
    UtRegisterTest("ConfGetNodeOrCreateTest", ConfGetNodeOrCreateTest, 1);
    UtRegisterTest("ConfNodePruneTest", ConfNodePruneTest, 1);
}

#endif /* UNITTESTS */