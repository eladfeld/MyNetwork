package bgu.spl.net.srv;

import bgu.spl.net.srv.StompExceptions.StompException;

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
            case "SEND":        return new StompCommand("SEND");
            case "SUBSCRIBE":   return new StompCommand("SUBSCRIBE");
            case "CONNECT":     return new StompCommand("CONNECT");
            case "DISCONNECT":  return new StompCommand("DISCONNECT");
            case "UNSUSCRIBE":  return new StompCommand("UNSUBSCRIBE");
            default: throw new StompException(cmd + " is not a valid STOMP command", cmd);
        }
    }
}
