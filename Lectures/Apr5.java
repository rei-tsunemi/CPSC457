public class Apr5 {
    public static void main(String[] args) {
        
    }
}

class HashTable {
    private class HashEntry {
        int element;    //data to be stored
        boolean isActive;

        HashEntry(int e) {
            this(e, true);
        }

        HashEntry(int e, boolean i) {
            element = e;
            isActive = i;
        }
    }

    private HashEntry[] array;
    private int capacity;
    private int curSize;
    private static int DEFAULT = 11;

    HashTable() {
        this(DEFAULT);
    }

    HashTable(int c) {
        capacity = c;
        curSize = 0;
        array = new HashEntry[capacity];
        for(int i =0; i < capacity; i++) {
            array[i] = null;
        }
        //initialize hash table and set each element to null
    }

    void insert(int e) {
        //first compute hash value h(e) & resolve collisions
        //then insert
        int pos = findPos(e);
        if(isActive(pos)) {
            return;
        }
        array[pos] = new HashEntry(e);
    }

    int findPosLinearProbing(int e) {
        int curPos = myHash(e);
        for(int i = 0; i < capacity; i++) {
            if(!array[curPos].isActive) {
                return curPos;
            }
            curPos = (curPos + 1) % capacity;
        }
        return curPos;
    }

    int myHash(int e) {
        return e % capacity;
    }

    boolean isActive(int pos) {
        return array[pos] != null && array[pos].isActive;
    }

    int findPos(int e) {
        int curPos = myHash(e);


        return curPos;
    }
}