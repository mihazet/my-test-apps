package com.yhlab.guessnumberhelper.guess.chooser;

import java.util.Random;
import com.yhlab.guessnumberhelper.guess.IChooserBuilder;
import com.yhlab.guessnumberhelper.guess.ChooserFactory;
import com.yhlab.guessnumberhelper.guess.GuessChooser;
import com.yhlab.guessnumberhelper.guess.ScoredChooser;

/**
 * <p>
 * An <code>InfoGainChooser</code> uses the information gain as the score of the
 * guess. Assume the number of candidates before the guess is n, and the guess
 * splits the node into m child nodes. The sizes of these child nodes are n1,
 * n2, ..., nm, respectively. Then the information gain is defined as
 * <blockquote>
 * 
 * <pre>
 * info_gain = n*log(n) - n1*log(n1) - ... - nm*log(nm)
 * </pre>
 * 
 * </blockquote>
 * </p>
 * 
 * <p>
 * However, for difference gusesses, the size of the node is the same, thus, the
 * formula can be simplified as <blockquote>
 * 
 * <pre>
 * info gain = -n1*log(n1) - n2*log(n2) - ... - nm*log(nm)
 * </pre>
 * 
 * </blockquote>. But if ni is one, then the cost of that group will be zero,
 * i.e., no cost. In order to give a differnce score between a child of size one
 * and the correct guess, we slidly change the formula to <blockquote>
 * 
 * <pre>
 * info gain = -n1*(log(n1)+w)-n2*(log(n1)+w)-...
 * </pre>
 * 
 * </blockquote> Where the "w" is the panilty of going to the next level,
 * experiments show that the value "1.8" is a good one for this parameter.
 * </p>
 * 
 * <p>
 * This formula can also explained as an estimator of the total depth of all the
 * candidates. The avarage guess times of this chooser is about 5.24.
 * </p>
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class InfoGainChooser extends ScoredChooser {
    static {
        ChooserFactory factory = ChooserFactory.getInstance();
        factory.registerBuilder("InfoGain", new Builder());
    }

    private int category[] = new int[0x100];
    private double weight;

    public InfoGainChooser(Random random, double weight) {
        super(random);
        this.weight = weight;
    }

    public InfoGainChooser(Random random) {
        this(random, 1.8);
    }

    public double score(int guess) {
        node.doLabelStatistics(guess, category);
        int n = node.getDigitCount();
        double sum = 0;
        for (int i = 0; i < n; ++i) { // 0xn0 is not for counting
            for (int j = 0, jn = n - i; j <= jn; ++j) {
                int x = category[(i << 4) | j];
                if (x == 0)
                    continue;
                sum += x * (Math.log(x) + weight);
            }
        }
        return -sum;
    }

    public String toString() {
        String name = this.getClass().getName();
        int index = name.lastIndexOf('.');
        StringBuilder builder = new StringBuilder(
                index < 0 ? name : name.substring(index + 1));
        builder.append("(").append(weight).append(")");
        return builder.toString();
    }

    private static class Builder implements IChooserBuilder {

        public GuessChooser buildChooser(String[] argv) {
            double weight = argv.length > 0 ?
                    Double.parseDouble(argv[0]) : 1.8;
            long seed = argv.length > 1 ?
                    Long.parseLong(argv[1]) :
                    System.currentTimeMillis();
            GuessChooser chooser = new InfoGainChooser(
                    new Random(seed), weight);
            System.out.println("build "
                    + chooser + " with random seed as " + seed);
            return chooser;
        }

    }

}
