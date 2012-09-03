package com.yhlab.guessnumberhelper.guess.chooser;

import java.util.Random;
import android.util.Log;
import com.yhlab.guessnumberhelper.guess.ChooserFactory;
import com.yhlab.guessnumberhelper.guess.GuessTreeNode;
import com.yhlab.guessnumberhelper.guess.IChooserCreater;
import com.yhlab.guessnumberhelper.guess.IGuessChooser;
import com.yhlab.guessnumberhelper.guess.Utility;

public class OneTwoThreeFourChooser implements IGuessChooser {

    static {
        ChooserFactory factory = ChooserFactory.getInstance();
        factory.registerChooser("OneTwoThreeFour", new Builder());
    }

    private static final String TAG = "OneTwoThreeFourChooser";
    private Random random;
    private int candidate[];

    public OneTwoThreeFourChooser(Random random) {
        if (random == null)
            throw new NullPointerException();
        this.random = random;
    }

    public int nextGuess(GuessTreeNode node) {
        if (this.candidate == null) {
            this.candidate = Utility.generateCandidates(
                    node.getDigitCount(), node.getSymbolCount());
        }
        if (node.size() <= candidate.length / 1000)
            return node.getNumber(0);
        int index = 0;
        int digitCount = node.getDigitCount();
        int symbolCount = node.getSymbolCount();

        int depth = node.getDepth();
        switch (depth) {
        case 1:
        case 2:

            for (int d = 0; d < digitCount - 1; d++) {
                int tempProduct = 1;
                int symbol = symbolCount;
                for (int d2 = d; d2 < digitCount - 1; d2++) {
                    tempProduct *= (symbol - d2 - 1);
                }
                index += tempProduct;
            }
            index++;

            if (depth == 2) {
                index *= (digitCount + 1);
            }

            break;

        default:
            index = random.nextInt(candidate.length);
            break;
        }
        return candidate[index];
    }

    public String toString() {
        String name = this.getClass().getName();
        int index = name.lastIndexOf('.');
        return index < 0 ? name : name.substring(index + 1);
    }

    private static class Builder implements IChooserCreater {

        public IGuessChooser createChooser(String[] argv) {
            long seed = argv.length == 0 ?
                    System.currentTimeMillis() :
                    Long.parseLong(argv[0]);
            Random random = new Random(seed);
            IGuessChooser chooser = new OneTwoThreeFourChooser(random);
            Log.d(TAG, chooser + " created with seed " + seed);
            return chooser;
        }

    }

}
