package bgu.spl.net.srv;

import bgu.spl.net.srv.StompExceptions.StompException;

import java.sql.SQLOutput;

public class StompCommand {

    String CommandType;

    public StompCommand(String cmd){
        CommandType=cmd;
    }

    //region getter and setter
    public String getCommandType() {
        return CommandType;
    }

    public void setCommandType(String commandType) {
        CommandType = commandType;
    }
    //endregion

    /**
     * process the given command and parse into the proper command type
     * @param cmd the received command
     */
    public static StompCommand parse(String cmd) throws StompException {
        switch (cmd){
            case "MESSAGE":     return new StompCommand("MESSAGE");
            case "RECEIPT":     return new StompCommand("RECEIPT");
            case "CONNECTED":   return new StompCommand("CONNECTED");
            case "ERROR":       return new StompCommand("ERROR");
            case "SEND":        return new StompCommand("SEND");
            case "SUBSCRIBE":   return new StompCommand("SUBSCRIBE");
            case "CONNECT":     return new StompCommand("CONNECT");
            case "DISCONNECT":  return new StompCommand("DISCONNECT");
            case "UNSUBSCRIBE":  return new StompCommand("UNSUBSCRIBE");
            default:
                System.out.println(cmd+" is not a valid command type");
                throw new StompException(cmd + " is not a valid STOMP command", cmd);
        }
    }
}
