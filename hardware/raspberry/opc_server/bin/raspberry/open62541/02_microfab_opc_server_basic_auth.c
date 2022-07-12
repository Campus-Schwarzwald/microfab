/* OPC UA Server with access control via user:password
 * The server generates three objects which represent the axis of a machine.
 * Each node has two subobjects: Cooling (Boolean) and Temperature (UInt16)
 * The basis of this script is the example shown in the open 62541 repository for an inheritance server:
 * https://github.com/open62541/open62541/blob/master/examples/access_control/server_access_control.c
 * author: Florian Handke (florian.handke@campus-schwarzwald.de)
 * date: 07-08-2022
 */

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/accesscontrol_default.h>

#include <signal.h>
#include <stdlib.h>

UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

//Acces control definition

static UA_UsernamePasswordLogin logins[2] = {
    {UA_STRING_STATIC("microfabuser"), UA_STRING_STATIC("microfoo123")},
    {UA_STRING_STATIC("microfabadmin"), UA_STRING_STATIC("admin")}
};

static UA_Boolean
allowAddNode(UA_Server *server, UA_AccessControl *ac,
             const UA_NodeId *sessionId, void *sessionContext,
             const UA_AddNodesItem *item) {
    printf("Called allowAddNode\n");
    return UA_TRUE;
}

static UA_Boolean
allowAddReference(UA_Server *server, UA_AccessControl *ac,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_AddReferencesItem *item) {
    printf("Called allowAddReference\n");
    return UA_TRUE;
}

static UA_Boolean
allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_DeleteNodesItem *item) {
    printf("Called allowDeleteNode\n");
    return UA_FALSE; // Do not allow deletion from client
}

static UA_Boolean
allowDeleteReference(UA_Server *server, UA_AccessControl *ac,
                     const UA_NodeId *sessionId, void *sessionContext,
                     const UA_DeleteReferencesItem *item) {
    printf("Called allowDeleteReference\n");
    return UA_TRUE;
}

/**
 * This method shows the usage of _begin and _finish methods.
 * Normally, if you create an instance of an object type, all its
 * mandatory children are inherited and created.
 * It could be the case that you first need to create a node,
 * add some children with specific IDs and then all the remaining
 * inherited children should be created.
 * For this use-case you can use first the _begin method,
 * which creates the node, including its parent references.
 * Then you can add any children, and then you should
 * call _finish on that node, which then adds all the inherited children.
 *
 * For further details check the example below or the corresponding
 * method documentation.
 *
 * To demonstrate this, we use the following example:
 *
 * + ObjectType
 *      + AxisType (Object)
 *          + IsOn (Variable, Boolean, Mandatory)
 *          + Temperature (Variable, UInt16, Mandatory)
 * + Objects
 *      + XAxis
 *          Should inherit the mandatory IsOn and Temperature with a generated node ID
 *      + YAxis
 *          IsOn should have the node ID 30101, Temperature will be inherited with a generated node ID
 *      + ZAxis
 *          IsOn should have the node ID 30101, Temperature will be inherited with a generated node ID
 *
 */
static void createCustomInheritance(UA_Server *server) {

    /* Add AxisType object type node */

    UA_ObjectTypeAttributes otAttr = UA_ObjectTypeAttributes_default;
    otAttr.description = UA_LOCALIZEDTEXT("en-US", "An Axis");
    otAttr.displayName = UA_LOCALIZEDTEXT("en-US", "AxisType");
    UA_Server_addObjectTypeNode(server, UA_NODEID_NUMERIC(1, 30000),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "AxisType"), otAttr, NULL, NULL);


    /* Add the two mandatory children, IsOn and Temperature */
    UA_VariableAttributes vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "Cooling");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "IsOn");
    UA_Boolean isOn = UA_FALSE;
    UA_Variant_setScalar(&vAttr.value, &isOn, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 30001),
                              UA_NODEID_NUMERIC(1, 30000),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "IsOn"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              vAttr, NULL, NULL);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(1, 30001),
                           UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);

    vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "Axis temperature");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "Temperature");
    UA_UInt16 temperature = 25;
    UA_Variant_setScalar(&vAttr.value, &temperature, &UA_TYPES[UA_TYPES_UINT16]);
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 30002),
                              UA_NODEID_NUMERIC(1, 30000),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "Temperature"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              vAttr, NULL, NULL);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(1, 30002),
                           UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);

    /* Now we want to inherit all the mandatory children for XAxis and don't care about the node ids.
     * These will be automatically generated. This will internally call the _begin and _finish methods */

    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.description = UA_LOCALIZEDTEXT("en-US", "The X axis");
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "XAxis");
    UA_Server_addObjectNode(server, UA_NODEID_NUMERIC(1, 0),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "XAxis"), UA_NODEID_NUMERIC(1, 30000),
                            oAttr, NULL, NULL);

    /* For the red lamp we want to set the node ID of the IsOn child manually, thus we need to use
     * the _begin method, add the child and then _finish: */

    /* The call to UA_Server_addNode_begin will create the node and its parent references,
     * but it will not instantiate the mandatory children */
    oAttr = UA_ObjectAttributes_default;
    oAttr.description = UA_LOCALIZEDTEXT("en-US", "The Y axis");
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "YAxis");
    UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
                            UA_NODEID_NUMERIC(1, 30100),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "YAxis"),
                            UA_NODEID_NUMERIC(1, 30000),
                            (const UA_NodeAttributes*)&oAttr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],
                            NULL, NULL);

    oAttr = UA_ObjectAttributes_default;
    oAttr.description = UA_LOCALIZEDTEXT("en-US", "The Z axis");
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ZAxis");
    UA_Server_addNode_begin(server, UA_NODECLASS_OBJECT,
                            UA_NODEID_NUMERIC(1, 30400),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "ZAxis"),
                            UA_NODEID_NUMERIC(1, 30300),
                            (const UA_NodeAttributes*)&oAttr, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],
                            NULL, NULL);

    /* Now we can add the IsOn with our own node ID */
    vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "Switch cooling on/off");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "IsOn");
    isOn = UA_FALSE;
    UA_Variant_setScalar(&vAttr.value, &isOn, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 30101),
                              UA_NODEID_NUMERIC(1, 30100),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "IsOn"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              vAttr, NULL, NULL);

    /* And then we need to call the UA_Server_addNode_finish which adds all the remaining
     * children and does some further initialization. It will not add the IsNode child,
     * since a child with the same browse name already exists */
    UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(1, 30100));
}

int main(void) {
    signal(SIGINT,  stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);

    createCustomInheritance(server);

    /* Disable anonymous logins, enable two user/password logins */
    config->accessControl.clear(&config->accessControl);
    UA_StatusCode retval = UA_AccessControl_default(config, false, NULL,
             &config->securityPolicies[config->securityPoliciesSize-1].policyUri, 2, logins);
    if(retval != UA_STATUSCODE_GOOD)
        goto cleanup;

    /* Set accessControl functions for nodeManagement */
    config->accessControl.allowAddNode = allowAddNode;
    config->accessControl.allowAddReference = allowAddReference;
    config->accessControl.allowDeleteNode = allowDeleteNode;
    config->accessControl.allowDeleteReference = allowDeleteReference;

    retval = UA_Server_run(server, &running);

 cleanup:
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}