import asyncio
import sys
# sys.path.insert(0, "..")
import logging
import time
from asyncua import Client, Node, ua
from asyncua.crypto.security_policies import SecurityPolicyBasic256Sha256

logging.basicConfig(level=logging.INFO)
_logger = logging.getLogger('asyncua')

cert = f"/app/certs/opc-client.cert.pem"
private_key = f"/app/certs/opc-client.key.pem"
server_cert = f"/app/certs/opc-server.cert.pem"

async def main():
    url = 'opc.tcp://192.168.88.101:4840/freeopcua/server/'
    # url = 'opc.tcp://commsvr.com:51234/UA/CAS_UA_Server'
    async with Client(url=url) as client:
        # Client has a few methods to get proxy to UA nodes that should always be in address space such as Root or Objects
        # Node objects have methods to read and write node attributes as well as browse or populate address space
        _logger.info('Children of root are: %r', await client.nodes.root.get_children())

        uri = 'http://examples.freeopcua.github.io'
        idx = await client.get_namespace_index(uri)
        # get a specific node knowing its node id
        # var = client.get_node(ua.NodeId(1002, 2))
        # var = client.get_node("ns=3;i=2002")
        var = await client.nodes.root.get_child(["0:Objects", f"{idx}:MyObject", f"{idx}:MyVariable"])
        print("My variable", var, await var.read_value())
        # print(var)
        # await var.read_data_value() # get value of node as a DataValue object
        # await var.read_value() # get value of node as a python builtin
        # await var.write_value(ua.Variant([23], ua.VariantType.Int64)) #set node value using explicit data type
        # await var.write_value(3.9) # set node value using implicit data type

async def browse_nodes(node: Node):
    """
    Build a nested node tree dict by recursion (filtered by OPC UA objects and variables).
    """
    node_class = await node.read_node_class()
    children = []
    for child in await node.get_children():
        if await child.read_node_class() in [ua.NodeClass.Object, ua.NodeClass.Variable]:
            children.append(
                await browse_nodes(child)
            )
    if node_class != ua.NodeClass.Variable:
        var_type = None
    else:
        try:
            var_type = (await node.read_data_type_as_variant_type()).value
        except ua.UaError:
            _logger.warning('Node Variable Type could not be determined for %r', node)
            var_type = None
    return {
        'id': node.nodeid.to_string(),
        'name': (await node.read_display_name()).Text,
        'cls': node_class.value,
        'children': children,
        'type': var_type,
    }

async def get_node_tree(url):
    try:
        client = Client(url=url)
        await client.set_security(
            SecurityPolicyBasic256Sha256,
            certificate=cert,
            private_key=private_key,
            server_certificate=server_cert
        )

        await client.connect()
        # Client has a few methods to get proxy to UA nodes that should always be in address space such as Root or Objects
        root = client.nodes.root
        _logger.info("Objects node is: %r", root)

        # Node objects have methods to read and write node attributes as well as browse or populate address space
        _logger.info("Children of root are: %r", await root.get_children())

        tree = await browse_nodes(client.nodes.objects)
        _logger.info('Node tree: %r', tree)
    except Exception as err:
        _logger.exception(err)
    finally:
        await client.disconnect()

async def read_data(url):
    try:
        client = Client(url=url)
        await client.set_security(
            SecurityPolicyBasic256Sha256,
            certificate=cert,
            private_key=private_key,
            server_certificate=server_cert
        )
        await client.connect()
        uri = 'http://examples.freeopcua.github.io'
        idx = await client.get_namespace_index(uri)
        _logger.info("Reading values from the server")
        _logger.info("### Reading value Objects --> MyObject --> MyVariable = NodeClass: 1 / NodeId: 50229")
        # Option 1 to read values
        var_opt_1 = await client.nodes.root.get_child(["0:Objects", f"{idx}:MyObject", f"{idx}:MyVariable"])
        _logger.info("**** Option 2: My variable: >>{}<<".format(await var_opt_1.read_value()))

        # Option 2 to read values
        #var_opt_2 = client.get_node("ns=2;i=2")
        #_logger.info("**** Option 2: My variable: >>{}<<".format(await var_opt_2.read_value()))

        # Option 3 to read values
        #var_opt_3 = client.get_node(ua.NodeId(2, 2))
        #_logger.info("**** Option 3: My variable: >>{}<<".format(await var_opt_3.read_value()))
    except Exception as err:
        _logger.exception(err)
    finally:
        await client.disconnect()

async def run_loop_forever(url):
    n = 0
    time_sleep = 4
    # await get_node_tree(url)
    while True:
        n = n + 1
        _logger.info(
            "\n##### This loop runs forever and reads every {} s the OPC UA values from the server. (n = {}) #####\n".format(
                time_sleep, n))
        await read_data(url)
        time.sleep(4)


if __name__ == "__main__":
    url = "opc.tcp://192.168.88.101:4840/freeopcua/server/"
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run_loop_forever(url))
    loop.run_forever()