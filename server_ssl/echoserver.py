import sys

from twisted.internet import reactor, ssl
from twisted.python import log
from twisted.web.server import Site
from twisted.web.static import File

from autobahn.twisted.websocket import WebSocketServerFactory, \
    WebSocketServerProtocol, \
    listenWS


class EchoServerProtocol(WebSocketServerProtocol):

    def onMessage(self, payload, isBinary):
        print payload;
        self.sendMessage(payload, isBinary)


if __name__ == '__main__':

    log.startLogging(sys.stdout)

    # SSL server context: load server key and certificate
    # We use this for both WS and Web!
    contextFactory = ssl.DefaultOpenSSLContextFactory('keys/server.key',
                                                      'keys/server.crt')

    factory = WebSocketServerFactory(u"wss://127.0.0.1:9000")

    factory.protocol = EchoServerProtocol
    listenWS(factory, contextFactory)

    webdir = File(".")
    webdir.contentTypes['.crt'] = 'application/x-x509-ca-cert'
    web = Site(webdir)
    # reactor.listenSSL(8080, web, contextFactory)
    reactor.listenTCP(8080, web)

    reactor.run()