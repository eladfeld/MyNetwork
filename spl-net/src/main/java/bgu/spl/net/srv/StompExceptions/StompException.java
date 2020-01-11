package bgu.spl.net.srv.StompExceptions;

public class StompException extends Exception {
    private String problem;
    private String stompMessage;

    public StompException(String problem, String stompMessage){
        super();
        this.problem = problem;
        this.stompMessage = stompMessage;
    }

    public String getProblem(){return  problem;}
    public String getMessage(){return stompMessage;}
}
