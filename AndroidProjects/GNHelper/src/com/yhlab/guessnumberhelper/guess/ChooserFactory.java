package com.yhlab.guessnumberhelper.guess;

import java.util.HashMap;
import java.util.NoSuchElementException;

/**
 * <p>
 * This class serves as the center point for users to register new builders and
 * create new instances of <code>GuessChooser</code> by these builders. This
 * class is designed as a singleton. Users can obtain the unique instance by the
 * static function <code>
 * getInstance()</code>. The instance will be created when this function is
 * called first time.
 * </p>
 * 
 * <p>
 * Users can provide new builders at runtime. When the instance is created, the
 * classes list in the following files would be also loaded. They are
 * <ul>
 * <li>"/META-INF/services/org.fattybobo.guess.ChooserBuilder",
 * <li>"$USER_HOME/org.fattybobo.guess.ChooserBuilder.list", or
 * <li>"$JAVA_HOME/lib/org.fattybobo.guess.ChooserBuilder.list".
 * </ul>
 * Each line of that file is the name of those classes to be loaded. Empty lines
 * and lines started with the symble '#' will be ignored. Usually the static
 * initializer of these class will register their builders to the
 * <code>ChooserFactory</code>. <blockquote>
 * 
 * <pre>
 * static {
 *     ChooserFactory factory = ChooserFactory.getInstance();
 *     factory.registerBuilder(&quot;Basic&quot;, new Builder());
 * }
 * </pre>
 * 
 * </blockquote>
 * </p>
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 * @see IChooserBuilder
 */
public class ChooserFactory {

    static private ChooserFactory singleton;

    private HashMap<String, IChooserBuilder> builderMap = new HashMap<String, IChooserBuilder>();

    private ChooserFactory() {
    }

    /**
     * Gets the instance of <code>ChooserFactory</code>. This class is designed
     * as a singleton. Users can obtain the unique instance by this static
     * function. The instance will be created at the first time this function is
     * called.
     * 
     * @return the singleton of <code>ChooserFactory</code>
     */
    public static ChooserFactory getInstance() {
        if (singleton == null) {
            singleton = new ChooserFactory();
            
            try {
                Class.forName("com.yhlab.guessnumberhelper.guess.chooser.BasicChooser");
                Class.forName("com.yhlab.guessnumberhelper.guess.chooser.InfoGainChooser");
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }
        }
        return singleton;
    }

    /**
     * Registers a builder with this factory. Users can use the registered
     * builder to create a new instance of <code>
     * GuessChooser</code> by calling the function <code>buildChooser</code>.
     * 
     * @param name
     *            the registered name
     * @param b
     *            the registered builder
     * @see #buildChooser(String, String[] )
     */
    public void registerBuilder(String name, IChooserBuilder b) {
        if (builderMap.containsKey(name))
            throw new IllegalStateException(
                    "alread has a builder named \"" + name + '"');
        builderMap.put(name, b);
    }

    /**
     * Builds a new instance of <code>GuessChooser</code> by the
     * <code>IChooserBuilder</code> which is registered with the given name.
     * 
     * @param name
     *            the registered name of the builder
     * @param argv
     *            the arguments for building the chooser
     * @return a new instance of <code>GuessChooser</code>
     */
    public IGuessChooser buildChooser(String name, String argv[]) {
        IChooserBuilder builder = (IChooserBuilder) builderMap.get(name);
        if (builder == null)
            throw new NoSuchElementException(name);
        if (argv == null)
            argv = new String[0];
        return builder.buildChooser(argv);
    }
}
