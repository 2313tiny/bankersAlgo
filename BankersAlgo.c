
#include<stdio.h>
#include<stdbool.h>

#define ROW	5
#define COLUMN  8  

#define RED 	"\033[01;031m"
#define YELLOW  "\033[01;033m"
#define GREEN 	"\033[01;032m"
#define RESET   "\033[01;0m"
#define CLEAR 	"\033[2J\033[1;1H"

void showMatrix(int (* a)[COLUMN], int (*b)[COLUMN] );
void calculateNeedMatrix( int  (* a)[COLUMN], int (* b)[COLUMN] );
void givesAvailableMatrix( int (* b)[COLUMN] );
void calculateAvailableMatrix( int (*a)[COLUMN], int  (*b)[COLUMN]
			       ,int *m_row , int * m_shift );
bool checkAvailableRow( int (*b)[COLUMN], int * m_row );
void findShift( int (*b)[COLUMN] ,int * m_row, int * m_shift );
void checkSafeState( int (*a)[COLUMN],	int (*b)[COLUMN] );
void checkAvailableRowState(bool *c, int * all_row);
void storyTeller1( void );
void storyTeller2( int n );
void initIsChecedAvailableRow( bool *c, int m_row);

#if 0
int a[ROW][COLUMN] = { {1, 1, 2, 4, 3, 3}
		   	, {2, 1, 2, 3, 2, 2} 
			, {4, 0, 1, 9, 0, 2} 
			, {0, 2, 0, 7, 5, 3} 
			, {1, 1, 2, 1, 1, 2} 
};
#endif 

int a[ROW][COLUMN] = { {0, 0, 1, 2, 0, 0, 1, 2}
		      ,{1, 0, 0, 0, 1, 7, 5, 0}
		      ,{1, 3, 5, 4, 2, 3, 5, 6}
		      ,{0, 6, 3, 2, 0, 6, 5, 2}
		      ,{0, 0, 1, 4, 0, 6, 5, 6}
};

int b[ROW][COLUMN];
static int curr_row = 0;     /*row which will be calculated in available part*/
static int active_row = 0;   /* row which will be calculated*/ 
static bool is_CheckedAvailableRow[ROW];

void initIsChecedAvailableRow( bool *c, int m_row)
{
	for (int i = 0; i < ROW; ++i) {
		*(c + i) = false;
	}
}

void checkAvailableRowState(bool *c, int * all_row)
{
	for (int i = 0; i < ROW; ++i) {
	       bool ans = *(c + i);
	       printf("[%d] - %s   ", i,  ans ? "true" : "false");
	}
	printf("\r\n");
}

void initB(int *b[COLUMN])
{
	for (int i = 0; i< ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j){
			*((*b+i)+j) = 0;
		}
	}
}

void showMatrix(int (* a)[COLUMN], int (*b)[COLUMN] )
{
	int delimiter = COLUMN / 2;
	const int WIDTH = COLUMN + 2;
	for (int i = 0; i < ROW; ++i) {
		if ( 0 == i) { 
			printf(YELLOW);
			printf("%20s %12s %15s", "Allocate", "Max", "Able");
			printf("%10s \r\n", "Need");
			printf(RESET);
		}
		printf("\t  ");
		for (int l = 0; l < COLUMN*4; ++l) {printf("-=");}
		printf(GREEN);
		printf("\r\nProcess [%d] | ", i);
		printf(RESET);

		for (int j = 0; j < 2*COLUMN; ++j) {
			if (j < COLUMN) {
				printf("%3d", *(*(a+i) + j));
				if ( 0 == (j +1) % delimiter) {
					printf(" | ");
				}
			} else { 
				int k = j - COLUMN ;
				printf("%3d", *(*(b+i) + k));
				if ( 0 == ( j+ 1 ) % delimiter) {
					printf(" | ");
				}
			}	
		}
		printf("\r\n"); /*end of every  line*/
		}
		printf("\r\n"); /*end of all matrix*/
}

void calculateNeedMatrix(int  (* a)[COLUMN], int (* b)[COLUMN])
{
	int z = COLUMN / 2;
	for (int i = 0; i < ROW; ++i) {
		int pivot = 0; 
		for (int j = 0; j < COLUMN; ++j ) {
			if (j > (z-1)) {
		    *(*(b+i) + j) = *(*(a+i) + z + pivot) -  *(*(a+i) + pivot);
			++pivot;
			}
		}
	}
}

void givesAvailableMatrix(int (* b)[COLUMN] )
{
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j) {
			*(*(b + i)+ j) = 0;
		}
	}	
	*(*(b + 0) + 0) = 1;  
	*(*(b + 0) + 1) = 5;
	*(*(b + 0) + 2) = 2;
	*(*(b + 0) + 3) = 0;

}

void calculateAvailableMatrix( int (*a)[COLUMN], int  (*b)[COLUMN]
			       ,int *m_row , int * m_shift)
{
	int half = COLUMN/2;

	for (int j = 0; j < half; ++j) {
		*(*(b+ *m_shift) + j ) =   
			*(*(b + *m_row ) + j) 
			+ *( *(a + *m_shift ) + j); 
	}
	is_CheckedAvailableRow[*m_shift] = true; 
	*m_row = *m_shift ; 
	active_row = *m_shift; 	
	printf(GREEN);
	printf("\r\n The system is at SAFE STATE - Process[%d]\r\n", *m_shift);
	printf(RESET);
}

bool checkAvailableRow( int (*b)[COLUMN], int * m_row)  
{
	printf("m_row = %d\r\n", *m_row);
	int half =  COLUMN/2 -1;
	for (int j = 0; j < half; ++j) {
		if (0 == *(*(b + *m_row) + j )) {
			j = COLUMN;
			return true; 	
		} else {
			return false; 	
		}
	}
}

/*[in] - m_row is current row, [out] m_shift which need matrix row is cmp*/
void findShift(int (*b)[COLUMN] ,int * m_row, int * m_shift)
{
	*m_shift = 0xBABA; /*not valid value, just for understand situation*/
	int pivot = COLUMN / 2;
	int underRow = *m_row;
	for (int i = underRow; i < ROW; ++i) {
	int count = 0;
		for (int j = 0; j < pivot; ++j) {
			if (*(*(b+ *m_row) + j) >= ( *(*(b+i)+j +pivot ))
				&& (false == is_CheckedAvailableRow[i]) ){
			++count;
			if ( (COLUMN/2) == count) { *m_shift = i;
					   i = ROW;			
			}
		} else { 
			j = pivot; 
		}	
		}
	}

	if (0xBABA == *m_shift) {underRow = 0; }
	for (int i = underRow; i < ROW; ++i) {
	int count = 0;
		for (int j = 0; j < pivot; ++j) {
			if (*(*(b+ *m_row) + j) >= ( *(*(b+i)+j +pivot ))
				&& (false == is_CheckedAvailableRow[i]) ){
			++count;
			if ( (COLUMN/2) == count) { *m_shift = i;
					   i = ROW;			
			}
		} else { 
			j = pivot; 
		}	
		}
	}
}


void checkSafeState( int (*a)[COLUMN],	int (*b)[COLUMN] )
{
	int shift = 0;
	int pivot = COLUMN / 2;
	bool is_empty = false;	

	is_empty = checkAvailableRow(b, &curr_row);
	if (false == is_empty ) {	
		findShift(b, &curr_row, &shift);
		calculateAvailableMatrix(a, b, &curr_row, &shift);	
		curr_row = shift-1;
	}
}

void storyTeller1(void)
{
	printf(CLEAR);
	printf("\r\n\t***************************************************");
	printf("\r\n\t   Consider the following snapshot of a system \r\n ");
	printf("\r\n\t***************************************************\r\n");
}

void storyTeller2( int n)
{
	printf("\r\n\t***************************************************");
	printf("\r\n\t\t   Content of the need matrix #%d \r\n ", n);
	printf("\r\n\t***************************************************\r\n");
}

int main(int argc, char * argv[])
{	
	/*show snapshot matrix*/
	storyTeller1();
	givesAvailableMatrix(b);
	initIsChecedAvailableRow(is_CheckedAvailableRow,ROW);
	showMatrix(a, b);
	
	storyTeller2(2);
	calculateNeedMatrix(a, b);
	showMatrix(a, b);
	int row = ROW;
	checkAvailableRowState(is_CheckedAvailableRow, &row);
	int m_shift = 0;
	findShift(b, &curr_row, &m_shift);
	calculateAvailableMatrix(a, b, &curr_row, &m_shift);	
	showMatrix(a, b);
	findShift(b, &curr_row, &m_shift);
	calculateAvailableMatrix(a, b, &curr_row, &m_shift);	
	showMatrix(a, b);
	checkAvailableRowState(is_CheckedAvailableRow, &row);
	findShift(b, &curr_row, &m_shift);
	calculateAvailableMatrix(a, b, &curr_row, &m_shift);	
	showMatrix(a, b);
	checkAvailableRowState(is_CheckedAvailableRow, &row);
	findShift(b, &curr_row, &m_shift);
	calculateAvailableMatrix(a, b, &curr_row, &m_shift);	
	showMatrix(a, b);
	checkAvailableRowState(is_CheckedAvailableRow, &row);
	findShift(b, &curr_row, &m_shift);
	calculateAvailableMatrix(a, b, &curr_row, &m_shift);	
	showMatrix(a, b);
	checkAvailableRowState(is_CheckedAvailableRow, &row);
	return 0;
}
