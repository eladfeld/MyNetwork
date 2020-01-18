package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if(args.length<1)throw new IllegalArgumentException("No argument given for server pattern");



        if(args[0].equals("TPC")){
            Server.threadPerClient(
                    7777,
                    ()->new StompProtocol(),
                    ()-> new EncoderDecoder()
            ).serve();
        }
        else {
            Server.reactor(
                    1,
                    7777,
                    ()->new StompProtocol(),
                    ()-> new EncoderDecoder()
            ).serve();
        }

    }


}
