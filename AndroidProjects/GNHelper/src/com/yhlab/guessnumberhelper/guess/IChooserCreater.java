package com.yhlab.guessnumberhelper.guess;

/**
 * Provides the interface to create a new instance of <code>GuessChooser</code>.
 * Users can create different types of builders to create different types of
 * <code>GuessChooser<code>.
 * By registering these builders with the <code>ChooserFactory
 * </code>, users can build these different <code>GuessChooser
 * </code>s directly from <code>ChooserFactory</code>.
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public interface IChooserCreater {

    /**
     * Create a new instance of <code>GuessChooser</code>. The instance is
     * created based on the builder and the the given arguments.
     * 
     * @param argv
     *            the arguments to build the chooser
     * @return a new <code>GuessChooser</code>
     */
    public IGuessChooser createChooser(String argv[]);
}
