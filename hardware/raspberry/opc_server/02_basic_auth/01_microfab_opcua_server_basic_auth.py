import logging
import asyncio
import sys
sys.path.insert(0, "../bin/raspberry")

from asyncua import ua, Server
from asyncua.common.methods import uamethod

@uamethod
def func(parent, value):
    return value * 2


async def main():
    _logger = logging.getLogger('microfab_opc_pwd_auth')
    # setup our server
    server = Server()
    await server.init()
    server.set_endpoint('opc.tcp://0.0.0.0:4840/freeopcua/server/')
    server.set_server_name("Microfab OPC UA Server")


    # setup our own namespace, not really necessary but should as spec
    uri = 'http://examples.freeopcua.github.io'
    idx = await server.register_namespace(uri)

    # populating our address space
    # server.nodes, contains links to very common nodes like objects and root
    myobj = await server.nodes.objects.add_object(idx, 'MyObject')
    myvar = await myobj.add_variable(idx, 'MyVariable', 2.0)
    # Set MyVariable to be writable by clients
    await myvar.set_writable()
    await server.nodes.objects.add_method(ua.NodeId('ServerMethod', 2), ua.QualifiedName('ServerMethod', 2), func, [ua.VariantType.Int64], [ua.VariantType.Int64])
    _logger.info('Starting server!')
    async with server:
        while True:
            await asyncio.sleep(1)
            act_val = await myvar.get_value()
            if act_val > 10:
                new_val = 2.0
            else:
                new_val = act_val + 0.1
            _logger.info('Pure password authentication is not implemented in asyncua: Set value of %s to %.1f', myvar, new_val)
            await myvar.write_value(new_val)


if __name__ == '__main__':

    logging.basicConfig(level=logging.DEBUG)

    asyncio.run(main(), debug=True)