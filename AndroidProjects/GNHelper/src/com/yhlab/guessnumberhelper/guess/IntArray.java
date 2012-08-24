package com.yhlab.guessnumberhelper.guess;

/**
 * This class is an extendable array implementation for the primitive type
 * <code>int</code>.
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class IntArray {
    private static final int DEFAULT_INIT_SIZE = 8;
    private int[] content;
    private int size = 0;

    /**
     * Constructs an instance of <code>IntArray</code> with totally
     * <code>size</code> elements. Please note that, the size of the created
     * array is still 0.
     * 
     * @param capacity
     *            the initial capacity
     */
    public IntArray(int capacity) {
        if (capacity < 0)
            throw new IllegalArgumentException();
        content = new int[capacity];
    }

    /**
     * Constructs an empty instance of <code>IntArray</code>.
     */
    public IntArray() {
        this(DEFAULT_INIT_SIZE);
    }

    /**
     * Empties the elements in this array. After clearing, the size of this
     * <code>IntArray</code> will be "0".
     */
    public void clear() {
        this.size = 0;
    }

    /**
     * Appends one elements to the tail of the <code>IntArray</code>. After this
     * operation, the size of the <code>IntArray</code> will be increased by
     * one.
     * 
     * @param value
     *            the value to be appended
     */
    public void add(int value) {
        if (size == content.length) {
            int buffer[] = new int[Math.max(4, size + size)];
            System.arraycopy(content, 0, buffer, 0, size);
            this.content = buffer;
        }
        content[size++] = value;
    }

    /**
     * Gets the value of the element of the given index.
     * 
     * @param index
     *            the index of the returned value
     * @return the value of the required element
     */
    public int get(int index) {
        if (index < 0 || index >= size)
            throw new ArrayIndexOutOfBoundsException(index);
        return content[index];
    }

    /**
     * Sets the value of the element of the given index.
     * 
     * @param index
     *            the index of the element to be set
     * @param value
     *            the value to be set
     */
    public void set(int index, int value) {
        if (index < 0 || index >= size)
            throw new ArrayIndexOutOfBoundsException(index);
        content[index] = value;
    }

    /**
     * Gets the size of this <code>IntArray</code>.
     * 
     * @return the size of this <code>IntArray</code>
     */
    public int size() {
        return size;
    }

    /**
     * Sets the size of this <code>IntArray</code>. If the new size is greater
     * than the current size, the new added elements will be all initialized as
     * "0". If the new size is smaller than the current size, the tail elements
     * will be truncated.
     * 
     * @param size
     *            the new size of this <code>IntArray</code>
     * @throws IllegalArgumentException
     *             if <code>size</code> &lt; 0
     */
    public void setSize(int size) {
        if (size < 0)
            throw new IllegalArgumentException();
        if (size == this.size)
            return;
        int buf[] = new int[size];
        System.arraycopy(this.content,
                0, buf, 0, Math.min(this.size, size));
        this.content = buf;
    }

    /**
     * Converts the content into a traditional <code>int</code> array.
     * 
     * @return an <code>int</code> array of the elements in this
     *         <code>IntArray</code>
     */
    public int[] toArray() {
        int ret[] = new int[size];
        System.arraycopy(content, 0, ret, 0, size);
        return ret;
    }

}
