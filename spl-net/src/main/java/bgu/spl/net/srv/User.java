package bgu.spl.net.srv;

import java.util.LinkedList;
import java.util.List;

public class User {
    private String login;
    private String passcode;
    private int connectionId; //=-1 if disconnected
    private boolean connected;

    public List<String> getTopics() {
        return topics;
    }

    public void setTopics(List<String> topics) {
        this.topics = topics;
    }

    private List<String> topics;

    public boolean isConnected() {
        return connected;
    }

    public void setConnected(boolean connected) {
        this.connected = connected;
    }


    public String getLogin() {
        return login;
    }

    public void setLogin(String login) {
        this.login = login;
    }

    public String getPasscode() {
        return passcode;
    }

    public void setPasscode(String passcode) {
        this.passcode = passcode;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }


    public User(String login, String passcode, int connectionId) {
        this.login = login;
        this.passcode = passcode;
        this.connectionId = connectionId;
        this.connected = true;
        this.topics = new LinkedList<>();
    }
}

