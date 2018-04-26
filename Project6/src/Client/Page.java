package Client;

public class Page {
    private String contentType;
    private String content;

    public Page(String contentType, String content) {
        this.contentType = contentType.replaceAll("^Content-Type: ", "");
        this.content = content;
    }

    public String getContentType() {
        return contentType;
    }

    public String getContent() {
        return content;
    }

}
