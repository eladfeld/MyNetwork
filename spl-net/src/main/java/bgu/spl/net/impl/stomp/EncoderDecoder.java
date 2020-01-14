package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;

import javax.xml.XMLConstants;
import java.io.Serializable;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class EncoderDecoder implements MessageEncoderDecoder<String> {

    private byte[] bytes = new byte[1 << 16];
    private int length = 0;


    @Override
    public String decodeNextByte(byte nextByte) {
        if (length == bytes.length) {
            bytes = Arrays.copyOf(bytes, length * 2);
        }
        bytes[length] = nextByte;
        length++;
        if (nextByte == 0) {
            String result = new String(bytes, 0, length, StandardCharsets.UTF_8);
            length = 0;
            return result;
        }
        return null;
    }

    @Override
    public byte[] encode(String message) {
        return message.getBytes();
    }
}
