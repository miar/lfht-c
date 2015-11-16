public class RunMe { 

    //static native void imprime();
    
    static {
	try {
	    System.loadLibrary("lfht");
	} catch (UnsatisfiedLinkError e) {
	    System.out.println("Native code library failed to load.\n" + e);
	    System.exit(1);
	}
    }
    
    public static void main(String[] args) {
	lfht e = new lfht();
	//	int f = e.fact(3);
	//	    System.out.println("Hello, World " + e.fact(3));
	
        e.dic_create_init_env();
	e.dic_check_insert_key(0, 0, e.dic_create_init_thread_env(0));
	e.dic_show_state("stdout");
	e.dic_abolish_all_keys();
	e.dic_show_state("stdout");

    }    
}
