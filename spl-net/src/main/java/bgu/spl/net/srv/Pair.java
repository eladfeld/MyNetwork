package bgu.spl.net.srv;

public class Pair<Q,E> {
    Q key;
    E value;
    public Pair(Q q, E e){
        key =  q;
        value = e;
    }
    public Q getKey(){
        return key;
    }
    public E getValue(){
        return value;
    }
}
