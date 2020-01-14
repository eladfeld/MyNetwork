package bgu.spl.net.srv;


import java.util.Map;

public interface Connections<T> {

    boolean connect(int connId, String login, String passCode);

    boolean send(int connectionId, T msg);

    boolean send(int connId,String channel, T msg, String messageId);

    boolean disconnect(int connectionId, String messageId);

    boolean subscribe(int id, String destination, String messageId);

    boolean unsubscribe(int id, String destination, String messageId);

    Map<Integer, ConnectionHandler<String>> getHandlers();
}
