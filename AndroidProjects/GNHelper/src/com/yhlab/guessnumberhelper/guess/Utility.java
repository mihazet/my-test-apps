package com.yhlab.guessnumberhelper.guess;

import java.util.Random;

/**
 * This class provide several usful static functions.
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class Utility {

    private Utility() {
    }

    /**
     * <p>
     * Get the label of the given candidate and guess. The label is the
     * relationship between the guess and the candidate. Those digits appear in
     * both numbers and locate at the same position is named as "A", and those
     * appear in both numbers but at different position is named "B". The label
     * is the number of "A"s and "B"s between the guess and the candidate.
     * </p>
     * 
     * <p>
     * For example, the label of "0123" and "0135" is "2A1B" (2A: 0 and 1, 1B:
     * 3). The value of label is represented in HEX formated. If the label is
     * "1A2B", the returned value will be 0x12.
     * 
     * @param candidate
     *            the candidate
     * @param guess
     *            the guess
     * @param digitCount
     *            the number of digits in the game
     * @return the label between the given candidate and the given guess
     */
    public static int label(int candidate, int guess, int digitCount) {
        int mask = 0x00, mark = 0;
        for (int i = 0; i < digitCount; ++i, candidate >>= 4, guess >>= 4) {
            int x = (candidate & 0x0F);
            int y = (guess & 0x0F);
            if (x == y) {
                mark += 0x10;
                continue;
            }
            int xbit = 0x01 << x, ybit = 0x01 << y;
            if ((mask & xbit) != 0)
                ++mark;
            if ((mask & ybit) != 0)
                ++mark;
            mask |= (xbit | ybit);
        }
        return mark;
    }

    /**
     * Gets the number of setted bits in the given <code>value
     * </code>. For example the bits count of 0x103F is 7.
     * 
     * @param value
     *            the value to be counted
     * @return the number of setted bits
     */
    public static int getBitsCount(int value) {
        int count = 0;
        while (value > 0) {
            if ((value & 0x01) != 0)
                ++count;
            value >>= 1;
        }
        return count;
    }

    /**
     * Generates all the candidate numbers in the game of the given digit count.
     * If the digit count is 4, the size of the returned array will be P(10,4) =
     * 5040.
     * 
     * @param digitCount
     *            the number of digits in the game
     * @return an array of all the candidate numbers
     */
    public static int[] generateCandidates(int digitCount, int symbolCount) {
        if (digitCount <= 0 || digitCount > 8)
            throw new IllegalArgumentException();
        if (symbolCount < digitCount || symbolCount >= 0x10)
            throw new IllegalArgumentException();
        int digit[] = new int[symbolCount];
        for (int i = 0; i < symbolCount; ++i)
            digit[i] = i;
        int size = 1;
        for (int i = 0; i < digitCount; ++i)
            size *= (symbolCount - i);

        IntArray array = new IntArray(size);
        generate(digitCount, array, digit, 0, 0);
        return array.toArray();
    }

    private static void generate(int n,
            IntArray array, int digit[], int depth, int number) {
        if (depth < n) {
            number <<= 4;
            int x = digit[depth];
            for (int i = depth, in = digit.length; i < in; ++i) {
                int t = digit[i];
                digit[i] = x;
                generate(n, array, digit, depth + 1, number | t);
                digit[i] = t;
            }
        } else
            array.add(number);
    }

    /**
     * Gets the bit mask of the given number. The bit mask will show which
     * digits are used in the given number. For example, the bit mask of number
     * "1357" will be 1010101(in binary), i.e, 0x55 (in HEX).
     * 
     * @param number
     *            the value to be calculated
     * @param digitCount
     *            the number of digits in the game
     * @return the bit mask of the given number
     */
    public static int getBitMask(int number, int digitCount) {
        int mask = 0x00;
        for (int j = 0; j < digitCount; ++j) {
            mask |= (0x01 << (number & 0x0F));
            number >>= 4;
        }
        return mask;
    }

    /**
     * Radomizes the sub content of the given array. The range of the array to
     * be randomized is started from the given offset for the given length.
     * 
     * @param a
     *            the array whose content will be randomized
     * @param offset
     *            the start point of the range to be randomized
     * @param length
     *            the length of the the range to be randomized
     * @param random
     *            the random variable used
     */
    public static void randomize(
            int a[], int offset, int length, Random random) {
        if ((offset | length) < 0 || offset + length > a.length)
            throw new IllegalArgumentException();
        for (; length > 0; --length) {
            int x = offset + random.nextInt(length);
            swap(a, x, offset + length - 1);
        }
    }

    /**
     * Randomizes the whole content of the given array. This function is
     * equivalent to <blockquote>
     * 
     * <pre>
     * randomize(a, 0, a.length, random);
     * </pre>
     * 
     * </blockquote>
     * 
     * @param a
     *            the array whose content will be randomized
     * @param random
     *            the random variable used
     */
    public static void randomize(int a[], Random random) {
        randomize(a, 0, a.length, random);
    }

    /**
     * Swaps the contents of two elements in the given array.
     * 
     * @param a
     *            the array whose two elements will be swapped
     * @param i
     *            the index of the first swapped element
     * @param j
     *            the index of the second swapped element
     */
    public static void swap(int[] a, int i, int j) {
        if (i == j)
            return;
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    /**
     * Converts the given number to <code>String</code>.
     * 
     * @param number
     *            the number to be converted
     * @param digitCount
     *            the number of digits
     * @return a <code>String</code> represent the given number
     */
    public static String toString(int number, int digitCount) {
        char ch[] = new char[digitCount];
        for (int i = digitCount - 1; i >= 0; --i) {
            int x = number & 0x0F;
            ch[i] = (char) (x < 10 ? '0' + x : 'A' + (x - 10));
            number >>= 4;
        }
        return new String(ch);
    }

    public static int factorial(int number) {
        if (number < 0)
            throw new IllegalArgumentException();
        int returned = 1;
        for (int i = 2; i <= number; ++i)
            returned *= i;
        return returned;
    }

    public static void reverse(int a[], int offset, int length) {
        int end = offset + length - 1;
        while (offset < end)
            swap(a, offset++, end--);
    }

    public static boolean nextPermutation(int a[]) {
        int index = a.length - 1;
        while (a[index] < a[index - 1])
            if (--index == 0)
                return false;
        reverse(a, index, a.length - index);
        int t = index - 1, target = a[t];
        while (a[index] < target)
            ++index;
        swap(a, t, index);
        return true;
    }

}
