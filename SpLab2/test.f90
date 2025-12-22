#define MAX_VAL 100
PROGRAM LexerTest
    IMPLICIT NONE

    INTEGER :: count = 42
    REAL :: velocity = 123.456
    REAL :: sci_num = 1.0E-4
    LOGICAL :: is_ready = .FALSE.
    CHARACTER(LEN=20) :: message = "Hello, World!"
    CHARACTER :: char_val = 'X'

    INTEGER :: hex_val = Z'1A'
    INTEGER :: bin_val = B'10110'
    INTEGER :: oct_val = O'755'

    TYPE Point
        INTEGER :: x, y
    END TYPE Point
    TYPE(Point) :: p

    INTEGER, ALLOCATABLE :: numbers(:)
    ALLOCATE(numbers(10))

    p%x = 10
    p%y = 20
    count = count + 1
    velocity = velocity ** 2
    velocity = velocity / 2.0

    IF (count > 10 .AND. is_ready .EQV. .FALSE.) THEN
        PRINT *, "Condition met"
    ELSEIF (count .LE. 5) THEN
        is_ready = .TRUE.
    ELSE
        message = 'Status: ' // "Unknown"
    END IF

    ! Цикли
    DO WHILE (count < MAX_VAL)
        count = count * 2
        IF (count == 84) EXIT
    END DO

    DEALLOCATE(numbers)

    ?

END PROGRAM LexerTest