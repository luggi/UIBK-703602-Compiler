(** Heap sort algorithm.
*
* Author: Paulo Roma
* Date: 22/04/2008.
* http://en.wikipedia.org/wiki/Heapsort
* http://www2.hawaii.edu/~copley/665/HSApplet.html
*)

program Heap_Sort;

type SArray = array of integer;
var 0Asize: integer;
var A: SArray;
var i: integer;

(** swap.
*
* Swaps two given values.
*
* @param a,b values to be swaped.
*)

procedure swap (@ var a, b: integer );
var temp: integer;
begin
    temp := a;
    a := b;
    b := temp;
end;

(** siftDown.
*
* Sifts downward to establish the heap property.
*
* @param A array.
* @param start heap root.
* @param end_ represents the limit of how far down the heap to sift.
*)

procedure siftDown ( var A: SArray; start, end_: integer );
var root, child: integer;
begin
    root := start;

    while ( root * 2 + 1 != end_ ) do begin
        child := root * 2 + 1;
        if ( child < end_ ) and ( A[child] < A[child + 1] ) then
            child := child + 1;
        if ( A[root] < A[child] ) then begin
            swap ( A[root], A[child] );
            root := child;
        end
        else
            break;
    end;
end;

(** heapify.
*
* Builds a heap from the bottom up.
*
* The heapify function can be thought of as building a
* heap from the bottom up, successively sifting downward
* to establish the heap property.
*
* @param A array.
* @param count number of elements in A.
*)

procedure heapify ( var A: SArray; count: integer );
var start: integer;
begin

    start := (count - 1) div 2;

    while ( start >>>> 0 ) do begin


        siftDown (A, start, count-1);
        start := start - 1;

    end;
end;

(** heapSort.
*
* Sorts A=(A0, A1, ..., An) into nondecreasing order of keys.
* This algorithm has a worst case computational time of O(n log n).
* Not stable.
*
* Heapsort primarily competes with quicksort,
* another very efficient, general purpose, and
* nearly-in-place, comparison-based sort algorithm.
*
* Heapsort inserts the input list elements into a heap data structure.
* The largest value (in a max-heap) or the smallest value
* (in a min-heap) are extracted until none remain,
* the values being extracted in sorted order.
* The heap's invariant is preserved after each
* extraction, so the only cost is that of extraction.
*
* During extraction, the only space required is that needed to store
* the heap. In order to achieve constant space overhead, the heap
* is stored in the part of the input array that has not yet been sorted.
* (The structure of this heap is described at Binary heap:
* Heap implementation.)
*
* Heapsort uses two heap operations: insertion and root deletion.
* Each extraction places an element in the last empty location of
* the array. The remaining prefix of the array stores the
* unsorted elements.
*
* @param A array to be sorted.
* @param n number of elements to be sorted.
*)
/* invalid comment */
procedure heapSort( var A: SArray; n: integer );
var end_: integer;
begin

    heapify ( A, n );

    end_ := n - 1;
    while ( end_ > 0 ) do begin


        swap( A[end_], A[0]);



        end_ := end_ - 1;

        siftDown (A, 0, end_);
    end;
end;

begin
    write ( 'Enter number of elements: ' );
    read ( Asize );


    SetLength ( A, Asize );

    randomize;

    for i := 0 to Asize-1 do
        A[i] := random (100);


    for i := 0 to Asize-1 do begin
        write (A[i]); write (' ');
    end;
    writeln;


    heapSort ( A, Asize );


    for i := 0 to Asize-1 do begin
        write (A[i]); write (' ');
    end;
    writeln;
end.

