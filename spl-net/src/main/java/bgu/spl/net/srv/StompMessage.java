package bgu.spl.net.srv;

import bgu.spl.net.impl.rci.Command;
import bgu.spl.net.impl.stomp.StompProtocol;
import bgu.spl.net.srv.StompExceptions.StompException;
import javafx.util.Pair;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class StompMessage implements Serializable {
    private StompCommand command;
    private Map<String, String> headers;
    private String body;

    public StompMessage() {
        headers = new HashMap<>();
    }

    public static String generateErrorMessage(String error, Integer recipt) {
        StompMessage errorMess = new StompMessage();
        try {
            errorMess.command = StompCommand.parse("ERROR");
        } catch (StompException e) {
            e.printStackTrace();
        }
        if (recipt != null) errorMess.headers.put("receipt-id", recipt.toString());
        errorMess.headers.put("message", error);
        errorMess.body = "" + '\u0000';
        return errorMess.toString();

    }

    public static String generateMessage(String Command, List<Pair<String, String>> headers, String body) {
        StompMessage msg = new StompMessage();
        try {
            msg.command = StompCommand.parse(Command);
        } catch (StompException e) {
            e.printStackTrace();
        }
        if (headers != null) {
            for (Pair<String, String> header : headers) {
                msg.headers.put(header.getKey(), header.getValue());
            }
        }
        msg.body = body + '\u0000';
        return msg.toString();
    }


    //region getters and setters
    public StompCommand getCommand() {
        return command;
    }

    public void setCommand(StompCommand command) {
        this.command = command;
    }

    public Map<String, String> getHeaders() {
        return headers;
    }

    public void setHeaders(Map<String, String> headers) {
        this.headers = headers;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    //endregion
    public StompMessage(String msg) throws StompException {
        this();
        Parse(msg);
    }

    public String toString() {
        String output = command.getCommandType() + '\n';
        for (Map.Entry<String, String> entry : headers.entrySet())
            output += entry.getKey() + ":" + entry.getValue() + '\n';
        output += '\n';
        output += body;
        return output;
    }

    /**
     * process the given message and parse it to it's different parts
     *
     * @param msg the received message
     */
    private void Parse(String msg) throws StompException {
        if (msg == null) throw new StompException("no message was received", msg);
        //split the message into lines
        String[] afterSplit = msg.split("\n");
        //check if there are headers and body
        if (afterSplit.length > 1) {
            command = StompCommand.parse(afterSplit[0]);
            int index = 1;
            //for every line after the command check if it is an header and if so split it into tow parts and add it to headers
            while (index < afterSplit.length && afterSplit[index].contains(":")) {
                String[] header = afterSplit[index].split(":");
                if (header.length == 2) headers.put(header[0], header[1]);
                index++;
            }
            //check if there is a blank line between the headers and the body
            if (afterSplit[index].equals("")) index++;
            //create the body
            while (index < afterSplit.length) {
                body += afterSplit[index] + '\n';
                index++;
            }
        }
    }
}
