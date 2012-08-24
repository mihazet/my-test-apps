package com.yhlab.guessnumberhelper.guess;

import java.util.Arrays;

/**
 * This class is used to solve the isomorphism problem. In some cases, one
 * guessing number is equivalent to
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class Automorphism {
    private static final int INITITAL_SIZE = 4;
    private static final int NODESIZE_LIMIT = 12;
    private static final int INVALID_PERMUTATION = -1;

    private GuessTreeNode core;
    private int others[];
    private Transform trans[] = new Transform[INITITAL_SIZE];
    private int transCount = 0;
    private int mask = 0x00;
    private int numbers[];

    public Automorphism(GuessTreeNode node) {
        this(node, buildDigitStat(node));
    }

    private static int[][] buildDigitStat(GuessTreeNode node) {
        int stat[][] = new int[
                node.getDigitCount()][node.getSymbolCount()];
        node.doDigitStatistics(stat);
        return stat;
    }

    public Automorphism(GuessTreeNode node, int stat[][]) {
        if (node == null || stat == null)
            throw new NullPointerException();
        this.core = node;
        int guesses[] = node.getGuessedNumbers();
        setupOthers(guesses);
        if (guesses.length <= 1 || node.size() <= NODESIZE_LIMIT) {
            setupTransform(guesses);
        } else {
            setupNumbers();
            setupTransform(stat);
            this.numbers = null;
        }
    }

    private boolean checkNumber(int value) {
        int n = core.getDigitCount(), mask = this.mask;
        int others[] = this.others, idx = 0;
        for (int i = n - 1; i >= 0; --i) {
            int x = (value >> (i << 2)) & 0x0F;
            if ((mask & (0x01 << x)) == 0)
                if (x != others[idx++])
                    return false;
        }
        return true;
    }

    private void setupNumbers() {
        int numb[] = new int[core.size()], count = 0;
        for (int i = 0, in = numb.length; i < in; ++i) {
            int v = core.getNumber(i);
            if (checkNumber(v))
                numb[count++] = v;
        }
        Arrays.sort(numb, 0, count);
        this.numbers = new int[count];
        System.arraycopy(numb, 0, this.numbers, 0, count);
    }

    private void setupOthers(int guesses[]) {
        int digitCount = core.getDigitCount(), symbolCount = core
                .getSymbolCount(), mask = 0x00;
        for (int i = 0, in = guesses.length; i < in; ++i) {
            int value = guesses[i];
            for (int j = 0; j < digitCount; ++j) {
                mask |= (0x01 << (value & 0x0F));
                value >>= 4;
            }
        }
        this.mask = mask;
        int o[] = this.others = new int[symbolCount], idx = 0;
        for (int i = 0, in = symbolCount; i < in; ++i)
            if ((mask & (0x01 << i)) == 0)
                o[idx++] = i;
    }

    private void setupTransform(int guesses[]) {
        int digitCount = core.getDigitCount();
        int count = guesses.length;
        int rotate[] = new int[digitCount];
        int changed[] = new int[count];
        for (int i = 0; i < digitCount; ++i)
            rotate[i] = i;
        findValidRotation(0, rotate, guesses, changed);
    }

    private void findValidRotation(int depth,
            int rotate[], int guesses[], int changed[]) {
        int digitCount = core.getDigitCount();
        int count = guesses.length;

        for (int i = depth; i < digitCount; ++i) {
            Utility.swap(rotate, i, depth);
            int r = rotate[depth] << 2, s = depth << 2;
            int eraseMask = ~(0x0F << s);
            for (int j = 0; j < count; ++j) {
                changed[j] &= eraseMask;
                changed[j] |= (((guesses[j] >> r) & 0x0F) << s);
            }
            int[] perm = getPermutation(guesses, changed, depth);
            if (depth + 1 == digitCount) {
                if (perm != null) {
                    addTransform(new Transform(
                            perm, (int[]) rotate.clone()));
                }
            } else if (perm != null) {
                findValidRotation(
                        depth + 1, rotate, guesses, changed);
            }
            Utility.swap(rotate, i, depth);
        }
    }

    private int[] getPermutation(
            int guesses[], int changed[], int depth) {
        int perm[] = new int[core.getSymbolCount()];
        int count = guesses.length;
        Arrays.fill(perm, INVALID_PERMUTATION);
        for (int j = 0; j < count; ++j) {
            int a = guesses[j], b = changed[j];
            for (int i = 0; i <= depth; ++i) {
                int s = a & 0x0F, t = b & 0x0F;
                if (perm[t] == INVALID_PERMUTATION) {
                    perm[t] = s;
                } else if (perm[t] != s)
                    return null;
                a >>= 4;
                b >>= 4;
            }
        }
        return perm;
    }

    private void addTransform(Transform transform) {
        if (transCount == trans.length) {
            Transform buffer[] = new Transform[transCount << 1];
            System.arraycopy(trans, 0, buffer, 0, transCount);
            this.trans = buffer;
        }
        this.trans[transCount++] = transform;
    }

    private void setupTransform(int stat[][]) {
        int digitCount = core.getDigitCount();
        int rotate[] = new int[digitCount];
        for (int i = 0; i < digitCount; ++i)
            rotate[i] = i;
        int id[] = setupIdentity(stat);
        findValidRotation(rotate, 0, stat, id);
    }

    private int[] setupIdentity(int stat[][]) {
        int digitCount = core.getDigitCount();
        int symbolCount = core.getSymbolCount();
        int identity[] = new int[digitCount];
        int temp[][] = new int[digitCount][symbolCount];
        for (int i = 0; i < digitCount; ++i) {
            identity[i] = i;
            System.arraycopy(stat[i], 0, temp[i], 0, symbolCount);
            Arrays.sort(temp[i]);
        }
        for (int i = 0; i < digitCount; ++i) {
            if (identity[i] != i)
                continue;
            for (int j = i + 1; j < digitCount; ++j) {
                if (isIdentical(temp[i], temp[j]))
                    identity[j] = i;
            }
        }
        return identity;
    }

    private static boolean isIdentical(int[] a, int[] b) {
        if (a.length != b.length)
            return false;
        for (int i = 0, in = a.length; i < in; ++i)
            if (a[i] != b[i])
                return false;
        return true;
    }

    private boolean checkTransform(Transform transform) {
        if (transCount == 0)
            return true;
        int numb[] = this.numbers;
        for (int i = 0, in = numb.length; i < in; ++i) {
            int t = transform.map(numb[i], others);
            if (Arrays.binarySearch(numb, t) < 0)
                return false;
        }
        return true;
    }

    private void findValidRotation(
            int[] rotate, int depth, int[][] stat, int id[]) {
        int digitCount = core.getDigitCount();
        if (depth == digitCount) {
            int symbolCount = core.getSymbolCount();
            int perm[] = new int[symbolCount], mask = this.mask;
            for (int i = 0; i < symbolCount; ++i)
                perm[i] = (mask & (0x01 << i)
                        ) == 0 ? INVALID_PERMUTATION : i;
            findValidPermutation(rotate, perm, 0, stat);
            return;
        }

        findValidRotation(rotate, depth + 1, stat, id);
        for (int i = depth + 1; i < digitCount; ++i) {
            Utility.swap(rotate, depth, i);
            if (id[i] == id[depth])
                findValidRotation(rotate, depth + 1, stat, id);
            Utility.swap(rotate, depth, i);
        }
    }

    private boolean checkPermutation(
            int target[], int stat[][], int idx) {
        int digitCount = core.getDigitCount();
        for (int i = 0; i < digitCount; ++i)
            if (target[i] != stat[i][idx])
                return false;
        return true;
    }

    private void findValidPermutation(
            int rotate[], int perm[], int depth, int[][] stat) {
        int symbolCount = core.getSymbolCount();
        int digitCount = core.getDigitCount();
        if (depth == symbolCount) {
            Transform t = new Transform(
                    (int[]) perm.clone(), (int[]) rotate.clone());
            if (checkTransform(t))
                addTransform(t);
            return;
        }
        if (perm[depth] == INVALID_PERMUTATION) {
            findValidPermutation(rotate, perm, depth + 1, stat);
            return;
        }
        int target[] = new int[digitCount];
        for (int i = 0; i < digitCount; ++i)
            target[i] = stat[rotate[i]][depth];
        for (int i = depth; i < symbolCount; ++i) {
            if (perm[i] == INVALID_PERMUTATION)
                continue;
            Utility.swap(perm, depth, i);
            if (checkPermutation(target, stat, perm[depth]))
                findValidPermutation(rotate, perm, depth + 1, stat);
            Utility.swap(perm, depth, i);
        }
    }

    public int getMinimalMapping(int guess) {
        if (trans.length == 1)
            return guess;
        int minimal = Integer.MAX_VALUE;
        for (int i = 0, in = transCount; i < in; ++i) {
            int x = trans[i].map(guess, this.others);
            if (x < minimal)
                minimal = x;
        }
        return minimal;
    }

    private static class Transform {
        private int[] permute;
        private int[] rotate;

        public Transform(int permute[], int rotate[]) {
            this.permute = permute;
            this.rotate = rotate;
        }

        public int map(int value, int others[]) {
            int r[] = this.rotate, p[] = this.permute;
            int returned = 0x00, index = 0, digitCount = r.length;

            for (int i = digitCount - 1; i >= 0; --i) {
                int x = p[(value >> (rotate[i] << 2)) & 0x0F];
                returned = ((returned << 4)
                        | (x < 0 ? others[index++] : x));
            }
            return returned;
        }
    }

}
