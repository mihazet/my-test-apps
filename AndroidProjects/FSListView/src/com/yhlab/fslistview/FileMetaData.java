package com.yhlab.fslistview;

public class FileMetaData {

    private String mName;
    private long mSize;
    private long mLastModifiedTime;
    private boolean mIsFile;
    private boolean mCanRead;
    private boolean mCanWrite;
    private boolean mCanExecute;

    /* Constructors */
    public FileMetaData(String name, long size, long time, boolean isFile,
            boolean read, boolean write, boolean execute) {
        mName = name;
        mSize = size;
        mLastModifiedTime = time;
        mIsFile = isFile;
        mCanRead = read;
        mCanWrite = write;
        mCanExecute = execute;
    }

    public FileMetaData() {
        this("", 0L, 0L, true, false, false, false);
    }

    /* setter and getter for name */
    public void setName(String name) {
        mName = name;
    }

    public String getName() {
        return mName;
    }

    /* setter and getter for size */
    public void setSize(long size) {
        mSize = size;
    }

    public long getSize() {
        return mSize;
    }

    /* setter and getter for last modified time */
    public void setlastModified(long time) {
        mLastModifiedTime = time;
    }

    public long getLastModified() {
        return mLastModifiedTime;
    }

    /* setter and getter for isFile */
    public void setIsFile(boolean isFile) {
        mIsFile = isFile;
    }

    public boolean getIsFile() {
        return mIsFile;
    }

    /* setter and getter for canRead */
    public void setCanRead(boolean can) {
        mCanRead = can;
    }

    public boolean getCanRead() {
        return mCanRead;
    }

    /* setter and getter for canWrite */
    public void setCanWrite(boolean can) {
        mCanWrite = can;
    }

    public boolean getCanWrite() {
        return mCanWrite;
    }

    /* setter and getter for canExecute */
    public void setCanExecute(boolean can) {
        mCanExecute = can;
    }

    public boolean getCanExecute() {
        return mCanExecute;
    }
}
