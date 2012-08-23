package com.yhlab.guessnumberhelper.guess;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * <p>The instance of <code>GuessTreeNode</code> represents a state 
 * of the guessing game. </p>
 * 
 * <p>Consider the case where the number of digits is four, 
 * there will be 5040 possible numbers at the begining. A new 
 * constructed node represents the initial state, we also call 
 * this node as the root of the game tree. The size of the node is 
 * defined as the number of possible numbers. Therefore, the size of 
 * the root node is 5040. After the first guess, for example, gussing 
 * "1234", there will be 14 different responses from the user, i.e., 
 * 0A0B, 0A1B, 0A2B, 0A3B, 0A4B, ....., 3A0B, 4A0B. Except for the 
 * case of 4A0B, we use different nodes for each different case. 
 * We say these 13 nodes are the children of the root node and 
 * the guessing number, i.e, 1234, is the <code>splitGuess</code> 
 * of the root node. And we say the root node is split. On the 
 * contrary, these 13 children are unsplit at this moment.</p>
 * 
 * <p>If the game tree has split in all node whose sizes are large 
 * than one. We say this tree is a COMPLETE game tree, because it has 
 * simulated all the cases of the users response. We can play the 
 * game with such complete tree. This is also the reason why this 
 * class has implemented the <code>Serializable</code> interface 
 * and can be serialized into permanent storage. You can use <code>
 * GuessTreeChooser</code> to use a tree to play a game.</p>
 *
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 * @see GuessChooser
 */
public class GuessTreeNode implements Serializable {
    public static final int NON_SPLIT = -1;
    private static final long 
            serialVersionUID = -1088587165312440165L;

    private static final 
            GuessTreeNode EMPTY_NODE[] = new GuessTreeNode[0];
    
    private GuessTreeNode parent;
    private GuessTreeNode child[] = null;
    private int splitGuess = NON_SPLIT;
    private int digitCount;
    private int symbolCount;
    
    private int numbers[];
    private int labels[];
    private int start;
    private int end;
    
    /**
     * Constructs a unsplit root node of the guessing game. The 
     * number of the digit should be in the range of (0, 8]. This 
     * is because we use the HEX format to represent the number. 
     * Otherwise, an exception of <code>IllegalArgumentException
     * </code> will be thrown.
     *  
     * @param digitCount the number of the digits of the game.
     */
    public GuessTreeNode( int digitCount, int symbolCount ) {
        if ( digitCount <= 0 || digitCount > 8 ) 
            throw new IllegalArgumentException();
        if ( symbolCount < digitCount || symbolCount >= 0x10 )
            throw new IllegalArgumentException();
        this.parent = null;
        this.digitCount = digitCount;
        this.symbolCount = symbolCount;
        this.numbers = Utility.
                generateCandidates( digitCount, symbolCount );
        this.labels = new int[ numbers.length ];
        this.start = 0;
        this.end = numbers.length;
    }
    
    private GuessTreeNode( 
            GuessTreeNode parent, int start, int end ) {
        this.parent = parent;
        this.numbers = parent.numbers;
        this.labels = parent.labels;
        this.digitCount = parent.digitCount;
        this.symbolCount = parent.symbolCount;
        this.start = start;
        this.end = end;
    }   
    
    /**
     * Gets the split guess of this node. If this node has not been
     * split yet, -1 (<code>NON_SPLIT</code>) will be returne.
     * 
     * @return the split guess number of this node or 
     *      <code>NON_SPLIT</code> if this node has not been split 
     */
    public int getSplitGuess() {
        return splitGuess;
    }
    
    /**
     * Gets the number of digits of the guessing game.
     * 
     * @return the number of digits of the guessing game
     */
    public int getDigitCount() {
        return digitCount;
    }
    
    public int getSymbolCount() {
        return symbolCount;
    }
        
    /**
     * Splits the node with the given gussing number 
     * <code>guess</code>. Please note that, It is allowed to 
     * re-split a split node. The previous split children will 
     * be discarded.
     * 
     * @param guess the number used to split the node
     */
    public void split( int guess ) {
        this.splitGuess = guess;
        int lab[] = this.labels, num[] = this.numbers;
        int s = start, e = end;
        for( int i=s; i < e; ++i )
            lab[i] = Utility.label( num[i], guess, digitCount );
        ArrayList<GuessTreeNode> list = new ArrayList<GuessTreeNode>();       
        while( true ) {
            int t = organize( lab[s], s + 1, e );
            if ( num[s] != guess ) {
                list.add( new GuessTreeNode( this, s, t ) );
            }
            if ( ( s = t ) == e ) break;
        }
        this.child = ( GuessTreeNode[] ) list.toArray( EMPTY_NODE );
    }
    
    /**
     * Gets the number of child nodes. An exception of type <code>
     * IllegalStateException</code> will be thrown if this node has 
     * not been split yet.
     * 
     * @return the number of the child nodes
     */
    public int getChildCount() {
        if ( child == null ) throw new IllegalStateException();
        return child.length;
    }
    
    /** 
     * Gets the child node of the given index. An exception of type 
     * <code>IllegalStateException</code> will be thrown if this node 
     * has not been split yet. If the index is not in the range of
     * [0, childCount), an exception of 
     * <code>ArrayIndexOutOfBoundsException</code> will be thrown.
     * 
     * @param index the index of the node required
     * @return the child node of the given index.
     */
    public GuessTreeNode getChild( int index ) {
        if ( child == null ) throw new IllegalStateException();
        return child[ index ];
    }   
    
    /** 
     * Gets the parent node of this node. If this node is the root
     * of the game tree, <code>null</code> will be returned.
     * 
     * @return the parent node of this node or <code>null</code>
     *      if this node is the root of the game tree
     */
    public GuessTreeNode getParent() {
        return parent;
    }
    
    private int organize( int target, int s, int e ) {
        int label[] = this.labels;
        int cand[] = this.numbers;
        if ( --e < s ) return e + 1;
        while( true ) {
            while( s < e && label[s] == target ) ++s;
            while( s < e && label[e] != target ) --e;
            if ( s >= e ) break;
            Utility.swap( label, s, e );
            Utility.swap( cand, s++, e-- );
        }       
        return s == e && label[s] == target ? s + 1 : s;
    }   
    
    /**
     * Gets the size of this node. The size of a node is defined as 
     * the possible candidates at the state represted by the node.
     * 
     * @return the size of this node.
     */
    public int size() {
        return end - start;
    }
    
    /** 
     * <p>Gets the possible candidate of the given index. The order 
     * of these possible candidates will be changed when any 
     * descendant of this node is split/resplit.</p> 
     * 
     * <p>The returned number is in HEX based. That is if the number
     * is "0123" the value is 0x0123 which is 291 in decimal format.
     * </p>
     * 
     * @param index the index of the returned candidate
     * @return the possible candidate of the given index
     * @throws ArrayIndexOutOfBounds if the given index is not in
     *      the range of [0, size()).
     */
    public int getNumber( int index ) {
        if ( index < 0 || index >= end - start )
            throw new ArrayIndexOutOfBoundsException( index );
        return numbers[ index + start ];
    }
        
    /** 
     * <p>Gets all the possible candidates of this node. The returned
     * <code>int</code> array is created inside the function and is
     * independent from the instance. </p>
     * 
     * <p>The returned number is in HEX based. That is if the number
     * is "0123" the value is 0x0123 which is 291 in decimal format.
     * </p>
     * 
     * return an array of all the possible candidates.
     */
    public int[] getNumbers() {
        int num[] = new int[ end - start ];
        System.arraycopy( numbers, start, num, 0, end - start );
        return num;
    }
    
    /**
     * Tests if this node has been split.
     * 
     * @return <code>true</code> if this node is split; 
     *      <code>false</code> otherwise.
     */
    public boolean isSplit() {
        return child != null;
    }
    
    public boolean isCompleteSplited() {
        if ( child == null ) return false;
        for( int i=0; i < child.length; ++i )
            if ( ! child[i].isCompleteSplited() ) return false;
        return true;
    }
    
    /**
     * <p>Gets the total depth of all candidate numbers in 
     * this node. The depth of a candidate number is defined as the 
     * depth of the node where the <code>splitNumber</code> is the 
     * candidate. The depth is counted from this node (not root) 
     * and is started from 1.</p>
     * 
     * <p> The depth of a candidate is also known as the number of 
     * guesses need to catch the candidate.</p> 
     * 
     * <p> For example, assume the candidate numbers in this node are
     * "0123", "0124" and "0142", and the split guess of this node is 
     * "0123". Under such assumption, this node has two child nodes. 
     * Then the total cost will be 1 + 2 + 2 = 5.</p> 
     *  
     * <p>This function can be called only if all the descendant have 
     * been split. Otherwise, an <code>IllegalStateException</code> 
     * will be thrown.</p>
     * 
     * @return the total depth of the the nodes in the tree.
     */
    public int getTotalDepth() {
        if ( child == null ) throw new IllegalStateException();
        int cost = size();
        for( int i=0; i < child.length; ++i )
            cost += child[i].getTotalDepth();
        return cost;            
    }
    
    private GuessTreeNode[] getPathNodes( int size ) {
        if ( parent == null ) {
            GuessTreeNode nodes[] = new GuessTreeNode[ size ];
            nodes[ size - 1 ] = this;
            return nodes;
        } else {
            GuessTreeNode nodes[] = parent.getPathNodes( size + 1 );
            nodes[ size - 1] = this;
            return nodes;
        }
    }
    
    /**
     * Gets the depth of the node. The depth is started from root 
     * and is counting from 1. For example the depth of the root 
     * node is 1.
     * 
     * @return the depth of the node
     */
    public int getDepth() {
        int count = 1;
        GuessTreeNode node = parent;
        while( node != null ) {
            ++ count;
            node = node.parent;
        }
        return count;
    }   
    
    /**
     * Gets the guessed numbers of this node. That are the <code>
     * splitGuess</code>s of the ancestors of this node. The 
     * returned guesses will follow the order of the path, i.e.,
     * the <code>splitGuess</code> will be the first element.
     * 
     * @return the guessed numbers of this node.
     */
    public int[] getGuessedNumbers() {
        GuessTreeNode nodes[] = getPathNodes( 1 );
        int length = nodes.length - 1;
        int guessed[] = new int[ length ];
        for( int i=0; i < length; ++i )
            guessed[i] = nodes[ length-i ].splitGuess;
        return guessed;
    }
    
    /**
     * Gets the digit mask of all the guessed numbers. If one digit 
     * is used in these guessed numbers the corresponding bit in the 
     * digit mask will be set. For example, if the guessed numbers 
     * is "0123", then the mask will be 0x000F. If the guessed 
     * numbers are "0123" and "4501", the bit mask will be 0x003F.
     * 
     * @return the digit mask of all the guessed number
     */
    public int getGuessedDigitMask() {
        GuessTreeNode nodes[] = getPathNodes( 1 );
        int length = nodes.length - 1;
        int mask = 0x00, fullMask = ( 0x01 << symbolCount ) - 1;
        for( int i=0; i < length; ++i ) {
            mask |= Utility.getBitMask( 
                    nodes[ length-i ].splitGuess, digitCount );
            if ( mask == fullMask ) break;
        }
        return mask;        
    }
    
    private void doDepthStatistic( IntArray array, int level ) {
        if ( child == null ) throw new IllegalStateException();
        int left = size();
        if ( child.length > 0 && array.size() <= level + 1 ) 
            array.add( 0 );
        for( int i=0; i < child.length; ++i ) {
            child[i].doDepthStatistic( array, level + 1 );
            left -= child[i].size();
        }
        if ( left > 1 ) throw new AssertionError( left );
        array.set( level, array.get( level ) + left );
    }
    
    /**
     * <p>Gets the statisitics on the depthes of all the candidates 
     * of this node. Please refer to <code>getTotalDepth()</code> 
     * for the definition of the depth of a candidate.</p>
     * 
     * <p>The returned <code>int</code> array represents the number 
     * of candidates of different depthes. For example, if the 
     * returned int array is { 1, 3, 5 }, that means there are 1 
     * candidate is of depth 1 is 1, and 3 candidates are of depth 2, 
     * and 5 candidate are of depth 3.</p> 
     * 
     * @return the statistics of the depthes of the candidates
     * @see #getTotalDepth()
     */
    public int[] getDepthStatistics() {
        IntArray array = new IntArray();
        array.add( 0 );
        doDepthStatistic( array, 0 );
        return array.toArray();
    }
        
    /**
     * <p>Finds the child of the specified label. In the game, the 
     * "label" is the response from users to indicate the relation
     * between guess and the answer. For example, the label between
     * candidate "0123" and the guess "0132" is 2A2B. The label is 
     * encoding as an integer, the value 0x11 means 1A1B; the value 
     * 0x22 means 2A2B and etc. Please refer to <code>Game</code> 
     * for more detail about the "label".</p>
     * 
     * <p>After spliting, all the candidates of the same label will 
     * be grouped as a child node. Thus, we define the label of these 
     * candidate as the label of the child node.</p>
     * 
     * <p>If there is no such child node of such label, <code>null
     * </code> will be returned.</p>
     * 
     * @param label the label of the returned child
     * @return the child node of the given label or null if no such 
     *      child node exists. 
     * @see Game
     */
    public GuessTreeNode findChild( int label ) {
        if ( child == null ) throw new IllegalStateException();
        for( int i=0; i < child.length; ++i ) {
            if ( Utility.label( numbers[ child[i].start ], 
                    splitGuess, digitCount ) == label ) 
                return child[i];
        }
        return null;
    }
    
    /**
     * <p>Performs the label statistics over the all candidates
     * in this node according to the given guess number. For the 
     * performance issue, client should prepare an int array of 
     * length at least 0x100(256) before calling this function.
     * Otherwise, an <code>IllegalArgumentException</code> will be
     * thrown.</p>
     * 
     * <p>After calling this function, the statistics result will be 
     * stored in the array <code>stat</code>. For example, if 
     * the value of <code>stat[0x10]<code> is 7, that means there are 
     * 7 candidates will be labeled as "1A0B" according to the given 
     * guess number.</p>
     * 
     * @param guess the guess number
     * @param stat the results of the statistics
     */ 
    public void doLabelStatistics( int guess, int stat[] ) {
        if ( stat.length < 0x100 ) 
            throw new IllegalArgumentException();
        for( int i=0; i <= digitCount; ++i ) {
            for( int j=0, jn = digitCount - i; j <= jn; ++j ) 
                stat[( i << 4 ) | j ] = 0;
        }
        int num[] = this.numbers;
        for( int i=start; i < end; ++i )
            ++ stat[ Utility.label( num[i], guess, digitCount ) ];
    }
    
    /**
     * Performs the digit statistics over all the candidates in 
     * this node. For the performance issue, clients should 
     * prepare a two layered <code>int</code> array of dimension of
     * <code>int[digitCount][symbolCount]</code> before calling this 
     * function. After calling this function, the statistics result
     * will be stored in the <code>stat</code> array. The value of 
     * stat[i][j] means the number of candidates where the digit 'i' 
     * appears in the position 'j'.  
     *  
     * @param stat the space to store the statistic result
     * @throws IllegalArgumentException if stat is not large enough
     *      to keep the statistics result
     */
    public void doDigitStatistics( int statistics[][] ) {
        if ( statistics.length != this.digitCount ) 
            throw new IllegalArgumentException();
        for( int i=0; i < digitCount; ++i ) {
            if ( statistics[i].length != symbolCount ) 
                throw new IllegalArgumentException();
            Arrays.fill( statistics[i], 0, symbolCount, 0 );
        }
        for( int i=start, in=end; i < in; ++i ) {
            int number = numbers[ i ];
            for( int j=0; j < digitCount; ++j ) {
                ++ statistics[ j ][ number & 0x0F  ];
                number >>= 4;
            }
        }
    }
    
    /**
     * Gets the digit mask of all the candidates in the node.
     * Please reference <code>getGuessedDigitMask()</code> for the 
     * definition of digit mask. For example, if the candidate 
     * numbers is "0123", then the mask is 0x000F. If the candidate 
     * numbers is "0123" adn "4501" then the mask will be 0x003F.
     *   
     * @return the digit mask of all the candidate numbers in the 
     *      node
     * @see #getGuessedDigitMask()
     */
    public int getNumberDigitMask() {
        int mask = 0, fullMask = ( ( 0x01 << symbolCount ) - 1 );
        for( int i=start, in=end; i < in; ++i ) {
            mask |= Utility.getBitMask( numbers[i], digitCount );
            if ( mask == fullMask ) break;
        }
        return mask;
    }

    /** 
     * Gets the label of the node. After spliting, all the candidate
     * numbers of the same label will be grouped as a child of the
     * split node. The same label of these candidates is also 
     * defined as the label of the child node. By this definition,
     * the root node has no label and calling this function will 
     * throw an <code>IllegalStateException</code>. 
     * 
     * @return the label of the node
     * @throws IllegalStateException if this node is the root node
     */
    public int getLabel() {
        if ( parent == null )
            throw new IllegalStateException( "no parent" );     
        return Utility.label( 
                numbers[start], parent.splitGuess, digitCount );
    }                           
    
    /**
     * Gets the index of the child. If the given node is not a child
     * of this node, -1 will be returned
     * 
     * @param node the node whose index will be returned
     * @return the index of the child, or -1 if the given node is 
     *      not a child of this node
     */
    public int getChildIndex( GuessTreeNode node ) {
        if ( node.parent != this ) return -1; 
        for( int i=0, n=child.length; i < n; ++i )
            if ( node == child[i] ) return i;
        return -1;
    }

    public static void saveTree( 
            GuessTreeNode root, DataOutputStream dos ) 
    throws IOException {
        if ( root.getParent() != null ) 
            throw new IllegalArgumentException( "not root" );
        dos.writeByte( root.getDigitCount() );
        dos.writeByte( root.getSymbolCount() );
        if ( root.isSplit() ) {
            dos.writeByte( root.getChildCount() );
            dos.writeInt( root.getSplitGuess() );
            for( int i=0, n=root.getChildCount(); i < n; ++i ) 
                saveTreeNode( root.getChild( i ), dos );
        } else dos.writeByte( -1 );
    }

    private static void saveTreeNode( 
            GuessTreeNode node, DataOutputStream dos ) 
    throws IOException {
        dos.writeByte( node.getLabel() );
        if ( node.isSplit() ) {
            dos.writeByte( node.getChildCount() );
            dos.writeInt( node.getSplitGuess() );
            for( int i=0, n=node.getChildCount(); i < n; ++i ) 
                saveTreeNode( node.getChild( i ), dos );            
        } else dos.writeByte( -1 );
    }

    public static GuessTreeNode loadTree( DataInputStream dis ) 
    throws IOException {
        int digitCount = dis.readByte();
        int symbolCount = dis.readByte();
        GuessTreeNode root = 
                new GuessTreeNode( digitCount, symbolCount );
        int childCount = dis.readByte();
        if ( childCount < 0 ) return root;        
        int splitGuess = dis.readInt();
        root.split( splitGuess );
        for( int i=0; i < childCount; ++i )
            loadTreeNode( root, dis );
        return root;
    }

    private static void loadTreeNode( 
            GuessTreeNode parent, DataInputStream dis ) 
    throws IOException {
        int label = dis.readByte();
        GuessTreeNode child = parent.findChild( label );
        if ( child == null ) 
            throw new IOException( "no such child node" );
        int childCount = dis.readByte();
        if ( childCount < 0 ) return;
        int splitGuess = dis.readInt();
        child.split( splitGuess );
        for( int i=0; i < childCount; ++i )
            loadTreeNode( child, dis );
    }    
    
}
