import logging
import asyncio
import sys
sys.path.insert(0, "../bin/raspberry")

from asyncua import ua, Server
from asyncua.common.methods import uamethod
from asyncua.crypto.permission_rules import SimpleRoleRuleset
from asyncua.server.users import UserRole
from asyncua.server.user_managers import CertificateUserManager


@uamethod
def func(parent, value):
    return value * 2


async def main():
    _logger = logging.getLogger('microfab_opc_pki')
    # setup our server
    cert_user_manager = CertificateUserManager()
    await cert_user_manager.add_user("/home/pi/build/certs/pki/opc-client.cert.der", name='microfab_opc_client')

    server = Server()
    await server.init()

    server.set_endpoint("opc.tcp://0.0.0.0:4840/freeopcua/server/")
    server.set_server_name("Microfab OPC UA Server")
    server.set_security_policy([ua.SecurityPolicyType.Basic256Sha256_SignAndEncrypt],
                               permission_ruleset=SimpleRoleRuleset())
    # load server certificate and private key. This enables endpoints
    # with signing and encryption.

    await server.load_certificate("/home/pi/build/certs/pki/opc-server.cert.der") # server certificate: .pem or .der
    await server.load_private_key("/home/pi/build/certs/pki/opc-server.key.pem")

    # setup our own namespace, not really necessary but should as spec
    uri = 'http://examples.freeopcua.github.io'
    idx = await server.register_namespace(uri)

    # populating our address space
    # server.nodes, contains links to very common nodes like objects and root
    myobj = await server.nodes.objects.add_object(idx, 'MyObject')
    myvar = await myobj.add_variable(idx, 'MyVariable', 6.7)
    # Set MyVariable to be writable by clients
    await myvar.set_writable()
    await server.nodes.objects.add_method(ua.NodeId('ServerMethod', 2), ua.QualifiedName('ServerMethod', 2), func, [ua.VariantType.Int64], [ua.VariantType.Int64])
    _logger.info('Starting server!')
    async with server:
        while True:
            await asyncio.sleep(1)
            new_val = await myvar.get_value() + 0.1
            _logger.info('Set value of %s to %.1f', myvar, new_val)
            await myvar.write_value(new_val)


if __name__ == '__main__':

    logging.basicConfig(level=logging.DEBUG)

    asyncio.run(main(), debug=True)