package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if(args.length < 1)throw new IllegalArgumentException("No argument given for server pattern");
        if(args.length < 2)throw new IllegalArgumentException("No port given for server pattern");
        int port = Integer.parseInt(args[1]);
        int numberOfThreads = 8;

        if(args[0].equals("TPC")){
            Server.threadPerClient(
                    port,
                    StompProtocol::new,
                    EncoderDecoder::new
            ).serve();
        }
        else {
            Server.reactor(
                    numberOfThreads,
                    port,
                    StompProtocol::new,
                    EncoderDecoder::new
            ).serve();
        }

    }


}
