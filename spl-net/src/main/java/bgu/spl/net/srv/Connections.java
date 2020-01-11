package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean connect(String login, String passCode);

    boolean send(int connectionId, T msg);

    boolean send(String channel, T msg, String messageId);

    boolean disconnect(int connectionId, String messageId);

    boolean subscribe(int id, String destination, String messageId);

    boolean unsubscribe(int id, String destination, String messageId);
}
