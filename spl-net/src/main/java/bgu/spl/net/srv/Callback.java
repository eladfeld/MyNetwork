package bgu.spl.net.srv;

public interface Callback<E, Q> {
    Q call​(E param);
}
