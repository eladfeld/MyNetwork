package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConnectionsImpl implements Connections<String> {

    private static class ConnectionsHolder {
        private static ConnectionsImpl instance = new ConnectionsImpl();
    }

    private Map<String, User> usersByLogin;
    private Map<Integer, User> usersById;
    private Map<Integer, ConnectionHandler<String>> handlers;
    private Map<String, Queue<Integer>> channels;


    private ConnectionsImpl() {
        usersById = new HashMap<>();
        usersByLogin = new HashMap<>();
        channels = new HashMap<>();
        handlers = new HashMap<>();
    }

    public static ConnectionsImpl getInstance() {
        return ConnectionsHolder.instance;
    }

    @Override
    public boolean connect(int connId, String login, String passCode) {
        if (usersByLogin.containsKey(login)) {
            User user = usersByLogin.get(login);
            if (user != null && user.isConnected()) {
                //user not logged in currently
                user.setConnectionId(connId);
                user.setConnected(true);
                usersById.put(connId, user);
            } else {
                //user already logged in
                handlers.get(connId).send(StompMessage.generateErrorMessage("user " + login + " have loged in already!", null));
                return false;
            }
        } else {
            //user does not exist (generate new one)
            User user = new User(login, passCode, connId);
            usersByLogin.put(login, user);
            usersById.put(connId, user);
        }
        handlers.get(connId).send(StompMessage.generateMessage("CONNECTED",null,""));
        return true;
    }

    @Override
    public boolean send(int connectionId, String msg) {
        if(!checkIfUserConnected(connectionId, null))return false;
        User user = usersById.get(connectionId);
        if(user == null || !user.isConnected())return false;
        ConnectionHandler handeler = handlers.get(connectionId);
        if (handeler == null) return false;
        handeler.send(msg);
        return true;
    }

    @Override
    public boolean send(int connId, String channel, String msg, String messageId) {
        if(!checkIfUserConnected(connId, null))return false;
        Queue<Integer> chanel = channels.get(channel);
        if (chanel == null) return false;
        for (Integer subscriberID : chanel) {
            if (subscriberID == null) return false;
            handlers.get(subscriberID).send(msg);
        }
        return true;
    }

    @Override
    public boolean disconnect(int connectionId, String messageId) {
        if(!checkIfUserConnected(connectionId, null))return false;
        User user = usersById.get(connectionId);
        for(String topic : user.getTopics())
            channels.get(topic).remove(connectionId);
        user.setConnected(false);
        usersById.remove(connectionId);
        return true;
    }


    @Override
    public boolean subscribe(int id, String channel, String messageId) {
        System.out.println("client subscibed to channel : " + channel + "!!");
        if(!checkIfUserConnected(id,null))return false;
        usersById.get(id).getTopics().add(channel);
        Queue<Integer> chanel = channels.get(channel);
        if (chanel == null) {
            chanel = new ConcurrentLinkedQueue<>();
            channels.put(channel, chanel);
        }
        chanel.add(id);
        return true;
    }

    @Override
    public boolean unsubscribe(int id, String channel, String messageId) {
        if(checkIfUserConnected(id, null))return false;
        if(!usersById.get(id).getTopics().remove(channel))return false;
        Queue<Integer> chanel = channels.get(channel);
        if (chanel == null) return false; //@TODO : generate a proper message and send it to the client
        chanel.remove(id);
        return true;
    }

    @Override
    public Map<Integer, ConnectionHandler<String>> getHandlers() {
        return handlers;
    }

    private boolean checkIfUserConnected(int id, Integer recipt){
        User user = usersById.get(id);
        if(user == null || !user.isConnected()){
            handlers.get(id).send(StompMessage.generateErrorMessage("user with id : " + id + " disconnected or doesn't exist!" , recipt));
            return false;
        }
        return true;
    }


}
