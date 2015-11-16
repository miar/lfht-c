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
	
	//e.LFHT_ThreadEnvPtr tenv;
        e.dic_create_init_env();

	SWIGTYPE_p_LFHT_ThreadEnvPtr tenv1 = e.dic_create_init_thread_env(1);
	SWIGTYPE_p_LFHT_ThreadEnvPtr tenv2 = e.dic_create_init_thread_env(2);

	e.dic_check_insert_key(0, 0, tenv1);
	e.dic_check_insert_key(1, 2, tenv2);

	e.dic_show_state("stdout");
	e.dic_abolish_all_keys();
	e.dic_show_state("stdout");

	e.dic_kill_thread_env(1);
	e.dic_kill_thread_env(2);
	e.dic_kill_env();
    }    
}
