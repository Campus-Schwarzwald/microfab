import sys

sys.path.insert(0, "../..")
import logging
import asyncio
import time

from asyncua import Client, Node, ua

logging.basicConfig(level=logging.INFO)
_logger = logging.getLogger('asyncua')


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


async def get_node_tree(url, sec_lvl):
    try:
        client = Client(url=url)
        if sec_lvl == 2:
            client.set_user()  # Set user name for the connection.
            client.set_password()  # Set user password for the connection
        elif sec_lvl == 3:
            client.set_security_string()

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

async def get_node_info(url):
    try:
        client = Client(url=url)
        root = client.get_root_node()
        objects = client.get_objects_node()
        namespace = await client.get_namespace_array()
        uri = 'http://opcfoundation.org/UA/'
        idx = await client.get_namespace_index(uri)

        _logger.info("**** Root node is: >>%r<<", root)
        _logger.info("**** Children of root are: >>%r<<", await root.get_children())
        _logger.info("**** Objects node is: >>%r<<", objects)
        _logger.info("**** Children of objects node are: >>%r<<", await objects.get_children())
        _logger.info("**** namespace: {}".format(namespace))
        _logger.info("**** index of our namespace is %s", idx)
    except Exception as err:
        _logger.exception(err)
    finally:
        await client.disconnect()

async def read_data(url, sec_lvl):
    try:
        client = Client(url=url)
        if sec_lvl == 2:
            client.set_user()  # Set user name for the connection.
            client.set_password()  # Set user password for the connection
        elif sec_lvl == 3:
            client.set_security_string()
            # Set SecureConnection mode.
            # :param string: Mode format ``Policy,Mode,certificate,private_key[,server_private_key]``
            # where:
            # - ``Policy`` is ``Basic128Rsa15``, ``Basic256`` or ``Basic256Sha256``
            # - ``Mode`` is ``Sign`` or ``SignAndEncrypt``
            # - ``certificate`` and ``server_private_key`` are paths to ``.pem`` or ``.der`` files
            # - ``private_key`` may be a path to a ``.pem`` or ``.der`` file or a conjunction of ``path``::``password`` where
            #   ``password`` is the private key password.

        await client.connect()

        _logger.info("Reading values from the server")
        _logger.info("### Reading value Objects --> LampGreen --> Brightness = NodeClass: 1 / NodeId: 50229")
        # Option 1 to read values
        var_opt_1 = await client.nodes.root.get_child(["0:Objects", "1:LampGreen", "1:Brightness"])
        _logger.info("**** Option 2: My variable: >>{}<<".format(await var_opt_1.read_value()))

        # Option 2 to read values
        var_opt_2 = client.get_node("ns=1;i=50229")
        _logger.info("**** Option 2: My variable: >>{}<<".format(await var_opt_2.read_value()))

        # Option 3 to read values
        var_opt_3 = client.get_node(ua.NodeId(50229, 1))
        _logger.info("**** Option 3: My variable: >>{}<<".format(await var_opt_3.read_value()))
    except Exception as err:
        _logger.exception(err)
    finally:
        await client.disconnect()

async def run_loop_forever(url, sec_lvl):
    n = 0
    time_sleep = 4
    await get_node_tree(url, sec_lvl)
    while True:
        n = n + 1
        _logger.info(
            "\n##### This loop runs forever and reads every {} s the OPC UA values from the server. (n = {}) #####\n".format(
                time_sleep, n))
        await read_data(url, sec_lvl)
        time.sleep(4)


if __name__ == "__main__":
    url = "opc.tcp://10.100.13.67:4840/freeopcua/server/"
    sec_lvl = 1
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run_loop_forever(url, sec_lvl))
    loop.run_forever()
