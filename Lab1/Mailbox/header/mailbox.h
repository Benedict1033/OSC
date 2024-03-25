#define MAILBOX_BASE        MMIO_BASE + 0xb880 // Base address for the mailbox interface

// Mailbox registers
#define MAILBOX_READ        ((volatile unsigned int *)(MAILBOX_BASE))
#define MAILBOX_STATUS      ((volatile unsigned int *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE       ((volatile unsigned int *)(MAILBOX_BASE + 0x20))

// Buffer and tag request code
#define REQUEST_CODE        0x00000000
#define TAG_REQUEST_CODE    0x00000000

// Buffer responde code
#define REQUEST_SUCCEED     0x80000000
#define REQUEST_FAILED      0x80000001

// Tags for identifier
#define GET_BOARD_REVISION  0x00010002
#define ARM_MEMORY          0x00010005

// Tag for end tag
#define END_TAG             0x00000000

// unknown
#define MAILBOX_RESPONSE    0x80000000
#define MAILBOX_EMPTY       0x40000000
#define MAILBOX_FULL        0x80000000

// Function declarations for mailbox operations.
int mailbox_call();
void get_board_revision();
void get_arm_memory();