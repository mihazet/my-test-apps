package com.yhlab.guessnumberhelper.guess.chooser;

import java.util.Random;
import android.util.Log;
import com.yhlab.guessnumberhelper.guess.ChooserFactory;
import com.yhlab.guessnumberhelper.guess.GuessTreeNode;
import com.yhlab.guessnumberhelper.guess.IChooserBuilder;
import com.yhlab.guessnumberhelper.guess.IGuessChooser;

/**
 * <p>
 * A <code>BasicChooser</code> randomly chooses one of the possible candidates
 * as the next guess.
 * </p>
 * 
 * <p>
 * Experiments show that it takes about 5.47 times to get the correct answer in
 * a 4-digit guess game.
 * </p>
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class BasicChooser implements IGuessChooser {

    static {
        ChooserFactory factory = ChooserFactory.getInstance();
        factory.registerBuilder("Basic", new Builder());
    }

    private static final String TAG = "BasicChooser";
    private Random random;

    public BasicChooser(Random random) {
        if (random == null)
            throw new NullPointerException();
        this.random = random;
    }

    public int nextGuess(GuessTreeNode node) {
        int index = random.nextInt(node.size());
        return node.getNumber(index);
    }

    public String toString() {
        String name = this.getClass().getName();
        int index = name.lastIndexOf('.');
        return index < 0 ? name : name.substring(index + 1);
    }

    private static class Builder implements IChooserBuilder {

        public IGuessChooser buildChooser(String[] argv) {
            long seed = argv.length == 0 ?
                    System.currentTimeMillis() :
                    Long.parseLong(argv[0]);
            Random random = new Random(seed);
            IGuessChooser chooser = new BasicChooser(random);
            Log.d(TAG, chooser + " created with seed " + seed); 
            return chooser;
        }

    }
}
