package com.yhlab.guessnumberhelper.guess;

/**
 * This class define only one function <code>nextGuess</code> which 
 * is used to select next guessing number under the given condition.
 * The condition of the guessing is the current state of the game,
 * which is the <code>GuessTreeNode</code> in our applications.
 *
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public interface GuessChooser {
    
    /** 
     * <p>Chooses the next guessing number. Sub classes of this 
     * class can implement this function to privide different 
     * strategies for choosing the number to guess. Please note 
     * that, the returned number MUST in Hex. For example, if next 
     * guess is "0123" then the number returned should be 0x0123
     * (in Hex), that is 291 (in Dec) instead of 123 (in Dec).</p>
     * 
     * <p>The given node is to represent the current state of the 
     * game. Please reference <code>GuessTreeNode</code> for more 
     * details. All the information related should be available 
     * from the given node, sub classes should choose the guessing 
     * number based on this information. 
     * 
     * @param node the current state of the game.
     * @return the next guessing number
     * @see GuessTreeNode
     */
    public int nextGuess( GuessTreeNode node );
    
}
