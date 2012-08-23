package com.yhlab.guessnumberhelper.guess;

import java.util.Arrays;

/**
 * This class is used the generate different guesses.
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class GuessGenerator {

    private GuessTreeNode node;
    private Automorphism isomorphism;

    private int uIndex;
    private int fIndex;
    private int index;
    private int unusedSymbol[];
    private int fixedSymbol[];
    private int positionSymbol[];
    private int symbol[];

    public GuessGenerator(GuessTreeNode node) {
        if (node == null)
            throw new NullPointerException();
        this.node = node;
        initialize();
    }

    public int[] generate() {
        if (node.size() <= 3)
            return node.getNumbers();
        IntArray array = new IntArray();
        uIndex = fIndex = index = 0;
        generate(array, 0, 0);
        return array.toArray();
    }

    private void generate(IntArray array, int depth, int number) {

        if (depth == this.node.getDigitCount()) {
            if (isomorphism != null) {
                int x = isomorphism.getMinimalMapping(number);
                if (x == number)
                    array.add(number);
            } else
                array.add(number);
            return;
        }

        int bound = fixedSymbol.length, n = node.getDigitCount() - 1;
        if (fIndex < bound) {
            if (positionSymbol[n - depth] >= 0) {
                int t = positionSymbol[n - depth], i = bound - 1;
                while (i > fIndex && fixedSymbol[i] != t)
                    --i;
                Utility.swap(fixedSymbol, fIndex, i);
            }
            int x = fixedSymbol[fIndex++];
            generate(array, depth + 1, (number << 4) | x);
            --fIndex;
        }

        for (int i = index, in = symbol.length; i < in; ++i) {
            int x = symbol[i];
            symbol[i] = symbol[index++];
            generate(array, depth + 1, (number << 4) | x);
            symbol[i] = x;
            --index;
        }

        if (uIndex < unusedSymbol.length) {
            int x = unusedSymbol[uIndex++];
            generate(array, depth + 1, (number << 4) | x);
            --uIndex;
        }

    }

    private void initialize() {
        int digitCount = node.getDigitCount();
        int symbolCount = node.getSymbolCount();

        positionSymbol = new int[node.getDigitCount()];
        Arrays.fill(positionSymbol, -1);

        int stat[][] = new int[digitCount][symbolCount];
        node.doDigitStatistics(stat);
        int fmask = getFixedDigitMask(stat);
        int gmask = node.getGuessedDigitMask();
        gmask &= ~fmask;
        symbol = new int[Utility.getBitsCount(gmask)];
        fixedSymbol = new int[Utility.getBitsCount(fmask)];
        unusedSymbol = new int[
                symbolCount - symbol.length - fixedSymbol.length];
        int index = 0, uindex = 0, findex = 0;
        for (int i = 0; i < symbolCount; ++i) {
            int bit = 0x01 << i;
            if ((bit & gmask) != 0)
                symbol[index++] = i;
            else if ((bit & fmask) != 0)
                fixedSymbol[findex++] = i;
            else
                unusedSymbol[uindex++] = i;
        }
        isomorphism = new Automorphism(node, stat);
    }

    private int getFixedDigitMask(int stat[][]) {
        int digitCount = node.getDigitCount();
        int symbolCount = node.getSymbolCount();
        int mask = 0x00, totalSize = node.size();
        for (int i = 0; i < symbolCount; ++i) {
            boolean noappear = true;
            for (int j = 0; j < digitCount; ++j) {
                int t = stat[j][i];
                if (t > 0)
                    noappear = false;
                if (t == totalSize) {
                    positionSymbol[j] = i;
                    mask |= (0x01 << i);
                    break;
                }
            }
            if (noappear)
                mask |= (0x01 << i);
        }
        return mask;
    }

}
