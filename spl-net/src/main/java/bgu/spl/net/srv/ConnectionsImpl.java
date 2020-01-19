package bgu.spl.net.srv;

import bgu.spl.net.srv.StompExceptions.StompException;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl implements Connections<String> {

    private static class ConnectionsHolder {
        private static ConnectionsImpl instance = new ConnectionsImpl();
    }

    private Map<String, User> usersByLogin;
    private Map<Integer, User> usersById;
    private Map<Integer, ConnectionHandler<String>> handlers;
    private Map<String, Queue<Integer>> channels;
    public AtomicInteger nextMessageId;


    private ConnectionsImpl() {
        usersById = new ConcurrentHashMap<>();
        usersByLogin = new ConcurrentHashMap<>();
        channels = new ConcurrentHashMap<>();
        handlers = new ConcurrentHashMap<>();
        nextMessageId = new AtomicInteger(1);
    }

    public static ConnectionsImpl getInstance() {
        return ConnectionsHolder.instance;
    }

    @Override
    public boolean connect(Integer connId, String login, String passCode) {
        if (usersByLogin.containsKey(login)) {
            User user = usersByLogin.get(login);
            if(user.isConnected()){
                handlers.get(connId).send(StompMessage.generateErrorMessage("user " + login + " is already logged in", null));
                return false;
            }
            if(!user.getPasscode().equals(passCode)) {
                handlers.get(connId).send(StompMessage.generateErrorMessage("user " + login + " provided the wrong passcode", null));
                return false;
            }
            if (user != null && !user.isConnected()) {
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
        handlers.get(connId).send(StompMessage.generateMessage("CONNECTED", new HashMap<String,String>(), ""));
        return true;
    }

    @Override
    public boolean send(int connectionId,String msg) {
        StompMessage message = null;
        try { message = new StompMessage(msg); } catch (StompException e){e.printStackTrace();}
        if (!checkIfUserConnected(connectionId, null)) return false;
        User user = usersById.get(connectionId);
        if (user == null || !user.isConnected()) return false;
        ConnectionHandler handeler = handlers.get(connectionId);
        if (handeler == null) return false;
        //String msg = StompMessage.generateMessage("MESSAGE", message.getHeaders(),message.getBody() );
        System.out.println(msg);
        handeler.send(msg);
        return true;
    }

    @Override
    public boolean send(int connId, String channel, String msg) {
        StompMessage message = null;
        try { message = new StompMessage(msg); } catch (StompException e){e.printStackTrace();}
        if (!checkIfUserConnected(connId, null)) return false;
        Queue<Integer> chanel = channels.get(channel);
        if (chanel == null) return false;
        for (Integer subscriberID : chanel) {
            if (subscriberID == null) return false;
            message.getHeaders().put("Message-id",""+nextMessageId.incrementAndGet());
            String msg1 = StompMessage.generateMessage("MESSAGE", message.getHeaders(), message.getBody());
            System.out.println(msg1);
            handlers.get(subscriberID).send(msg1);
        }
        return true;
    }

    @Override
    public boolean disconnect(int connectionId, String receiptId) {
        if (!checkIfUserConnected(connectionId, null)) return false;
        User user = usersById.get(connectionId);
        for (String topic : user.getTopics())
            channels.get(topic).remove(connectionId);
        user.setConnected(false);
        usersById.remove(connectionId);
        boolean output =  addHeaderAndSend(connectionId, receiptId);
        ConnectionHandler userHandler =handlers.get(connectionId);
        return output;
    }


    @Override
    public boolean subscribe(int id, String channel, String receiptId) {
        System.out.println("client subscibed to channel : " + channel + "!!");
        if (!checkIfUserConnected(id, null) | receiptId == null | channel == null) return false;
        usersById.get(id).getTopics().add(channel);
        Queue<Integer> chanel = channels.get(channel);
        if (chanel == null) {
            chanel = new ConcurrentLinkedQueue<>();
            channels.put(channel, chanel);
        }
        chanel.add(id);
        return addHeaderAndSend(id, receiptId);
    }

    private boolean addHeaderAndSend(int id, String receiptId) {
        Map<String, String> headers = new HashMap<>();
        headers.put("receipt-id",receiptId);
        handlers.get(id).send(StompMessage.generateMessage("RECEIPT",headers,""+'\u0000').toString());
        return true;
    }

    @Override
    public boolean unsubscribe(int id, String channel, String receiptId) {
        System.out.println("client unsubscibed to channel : " + channel + "!!");
        if (!checkIfUserConnected(id, null)) return false;
        if (!usersById.get(id).getTopics().remove(channel)) return false;
        Queue<Integer> chanel = channels.get(channel);
        if ( receiptId == null | channel == null) return false; //@TODO : generate a proper message and send it to the client
        chanel.remove(id);
        return addHeaderAndSend(id, receiptId);
    }

    @Override
    public Map<Integer, ConnectionHandler<String>> getHandlers() {
        return handlers;
    }

    private boolean checkIfUserConnected(int id, Integer recipt) {
        User user = usersById.get(id);
        if (user == null || !user.isConnected()) {
            handlers.get(id).send(StompMessage.generateErrorMessage("user with id : " + id + " disconnected or doesn't exist!", recipt));
            return false;
        }
        return true;
    }


}
