package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Callback;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.StompExceptions.StompException;
import bgu.spl.net.srv.StompMessage;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;


//Select the project, then File > ProjectStructure > ProjectSettings > Modules -> sources
public class StompProtocol implements StompMessagingProtocol {
    private boolean shouldTerminate = false;
    private Integer connectionId;
    private Connections<String> connections;
    private Map<String, Callback<StompMessage, Boolean>> commandResponses;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        if (commandResponses == null) initCommandResponses();
        this.connectionId = connectionId;
        this.connections = connections;
    }
    private Integer getConnectionId(){
        return connectionId;
    }

    //check for thread safety
    private void initCommandResponses() {

        commandResponses = new HashMap<String, Callback<StompMessage, Boolean>>();
        commandResponses.put("CONNECT", (msg) -> {
            return connections.connect(getConnectionId(), msg.getHeaders().get("login"), msg.getHeaders().get("passcode"));
        });
        commandResponses.put("SEND", (msg) -> {
            return connections.send(connectionId, msg.getHeaders().get("destination"), msg.toString());
        });
        commandResponses.put("SUBSCRIBE", (msg) -> {
            return connections.subscribe(connectionId, msg.getHeaders().get("destination"), msg.getHeaders().get("receipt"));
        });
        commandResponses.put("UNSUBSCRIBE", (msg) -> {
            return connections.unsubscribe(connectionId, msg.getHeaders().get("destination"), msg.getHeaders().get("receipt"));
        });
        commandResponses.put("DISCONNECT", (msg) -> {
            shouldTerminate = true;
            return connections.disconnect(connectionId, msg.getHeaders().get("receipt"));
        });
    }

    @Override
    public void process(String msg) {
        System.out.println(msg);
        StompMessage message = null;
        try {
            message = new StompMessage(msg);
            String command = message.getCommand().getCommandType();
            commandResponses.get(command).call​(message);
        } catch (StompException stompy) {
            handleProblem(stompy);
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    private void handleProblem(StompException stompy) {
        String toSend = "ERROR" + '\n';
        toSend += "message: " + stompy.getProblem() + '\n';
        if (stompy.getMessage() == null) {
            toSend += '\n' + '\u0000';
            connections.send(connectionId, toSend);
            return;
        }

        if (stompy.getMessage().contains("Message-id:")) {
            int idx = stompy.getMessage().indexOf("Message-id:") + 11;
            char c = stompy.getMessage().charAt(idx);
            String MessageID = "receipt-id:" + c;
            while (c != '\n') {
                MessageID += c;
                idx++;
                c = stompy.getMessage().charAt(idx);
            }
            toSend += MessageID + '\n';
        }
        toSend += '\n' + "ORIGINAL MESSAGE\n----------\n" + stompy.getMessage();
        connections.send(connectionId, toSend);
    }
}
