
public class UseExample { 

    //static native void imprime();
    
    static {
	try {
	    System.loadLibrary("example");
	} catch (UnsatisfiedLinkError e) {
	    System.out.println("Native code library failed to load.\n" + e);
	    System.exit(1);
	}
    }
    
    public static void main(String[] args) {
	example e = new example();
	//	int f = e.fact(3);
	//	    System.out.println("Hello, World " + e.fact(3));
	
        e.imprime();
    }


    
}
