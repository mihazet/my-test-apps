package com.yhlab.guessnumberhelper.guess;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.NoSuchElementException;

/**
 * <p>This class serves as the center point for users to register new 
 * builders and create new instances of <code>GuessChooser</code> 
 * by these builders. This class is designed as a singleton. Users 
 * can obtain the unique instance by the static function <code>
 * getInstance()</code>. The instance will be created when this 
 * function is called first time.</p>
 * 
 * <p>Users can provide new builders at runtime. When the instance 
 * is created, the classes list in the following files would be 
 * also loaded. They are <ul>
 * <li>"/META-INF/services/org.fattybobo.guess.ChooserBuilder", 
 * <li>"$USER_HOME/org.fattybobo.guess.ChooserBuilder.list", 
 * or <li>"$JAVA_HOME/lib/org.fattybobo.guess.ChooserBuilder.list". 
 * </ul>Each line of that file is the name of those classes to be 
 * loaded. Empty lines and lines started with the symble '#' will be 
 * ignored. Usually the static initializer of these class will 
 * register their builders to the <code>ChooserFactory</code>. 
 * <blockquote><pre>
 * static {
 *     ChooserFactory factory = ChooserFactory.getInstance();
 *     factory.registerBuilder( "Basic", new Builder() );
 * }    
 * </pre></blockquote></p>
 *
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 * @see ChooserBuilder
 */
public class ChooserFactory {
    
    static private ChooserFactory singleton;
    
    private HashMap builderMap = new HashMap();

    private ChooserFactory() {      
    }
    
    /**
     * Gets the instance of <code>ChooserFactory</code>. This class
     * is designed as a singleton. Users can obtain the unique 
     * instance by this static function. The instance will be 
     * created at the first time this function is called.
     *  
     * @return the singleton of <code>ChooserFactory</code>
     */
    public static ChooserFactory getInstance() {
        if ( singleton == null ) {
            singleton = new ChooserFactory();
            singleton.loadChooserBuilders();
        }
        return singleton;
    }
    
    /**
     * Registers a builder with this factory. Users can use the 
     * registered builder to create a new instance of <code>
     * GuessChooser</code> by calling the function 
     * <code>buildChooser</code>. 
     * 
     * @param name the registered name
     * @param b the registered builder
     * @see #buildChooser( String, String[] )
     */
    public void registerBuilder( String name, ChooserBuilder b ) {
        if ( builderMap.containsKey( name ) )
            throw new IllegalStateException( 
                    "alread has a builder named \"" + name + '"' );
        builderMap.put( name, b );
    }
    
    /**
     * Builds a new instance of <code>GuessChooser</code> by the 
     * <code>ChooserBuilder</code> which is registered with the given
     * name.
     * 
     * @param name the registered name of the builder
     * @param argv the arguments for building the chooser
     * @return a new instance of <code>GuessChooser</code>
     */
    public GuessChooser buildChooser( String name, String argv[] ) {
        ChooserBuilder builder 
                = (ChooserBuilder) builderMap.get( name );
        if ( builder == null )
            throw new NoSuchElementException( name );
        if ( argv == null ) argv = new String[ 0 ];
        return builder.buildChooser( argv );
    }
    
    private void loadJarBuilders() throws IOException {
        String factoryId = ChooserBuilder.class.getName();
        String serviceId = "META-INF/services/" + factoryId;
        ClassLoader loader = ChooserFactory.class.getClassLoader();
        InputStream is = loader.getResourceAsStream( serviceId );
        if ( is == null ) return;
        try {
            loadChooserBuilders( is );
        } finally {
            is.close();
        }
    }
    
    private void loadChooserBuilders() {
        try {
            loadSystemBuilders();            
        } catch( Exception e ) {
            e.printStackTrace();
        }
        try {
            loadUserBuilders();          
        } catch( Exception e ) {
            e.printStackTrace();
        }
        try {
            loadJarBuilders();           
        } catch( Exception e ) {
            e.printStackTrace();
        }
        try {
            Class.forName("org.fattybobo.guess.chooser.BasicChooser");
            Class.forName("org.fattybobo.guess.chooser.InfoGainChooser");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
    
    private void loadSystemBuilders() throws IOException {
        String filename = ChooserBuilder.class.getName() + ".list";
        String path = System.getProperty( "java.home" ) 
                + File.separator + "lib" + File.separator + filename;
        File file = new File( path );
        if ( ! file.exists() ) return;
        FileInputStream fis = new FileInputStream( file );
        try {
            loadChooserBuilders( fis );
        } finally {
            fis.close();
        }
    }
    
    private void loadUserBuilders() throws IOException {
        String filename = ChooserBuilder.class.getName() + ".list";
        String path = System.getProperty( 
                "user.home" ) + File.separator + filename;
        File file = new File( path );
        if ( ! file.exists() ) return;
        FileInputStream fis = new FileInputStream( file );
        try {
            loadChooserBuilders( fis );
        } finally {
            fis.close();
        }
    }
    
    private void loadChooserBuilders( InputStream is ) 
    throws IOException {
        BufferedReader br = 
            new BufferedReader( new InputStreamReader( is ) );
        String line = null;
        while( ( line = br.readLine() ) != null ) {
            line = line.trim();
            if ( line.startsWith( "#" ) || line.length() == 0 ) 
                continue;
            try {
                Class.forName( line );
            } catch ( ClassNotFoundException e ) {
                System.err.println( "no such builder - " + line );
            }
        }
    }   
    
}

