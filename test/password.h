
#define NUM_1_LOW_X 0
#define NUM_1_LOW_Y 250
#define NUM_1_HIGH_X 217
#define NUM_1_HIGH_Y 330

#define NUM_2_LOW_X 217
#define NUM_2_LOW_Y 250
#define NUM_2_HIGH_X 434
#define NUM_2_HIGH_Y 330

#define NUM_3_LOW_X 434
#define NUM_3_LOW_Y 250
#define NUM_3_HIGH_X 651
#define NUM_3_HIGH_Y 330

#define NUM_4_LOW_X 0
#define NUM_4_LOW_Y 330
#define NUM_4_HIGH_X 217
#define NUM_4_HIGH_Y 410

#define NUM_5_LOW_X 217
#define NUM_5_LOW_Y 330
#define NUM_5_HIGH_X 434
#define NUM_5_HIGH_Y 410

#define NUM_6_LOW_X 434
#define NUM_6_LOW_Y 330
#define NUM_6_HIGH_X 651
#define NUM_6_HIGH_Y 410

#define NUM_7_LOW_X 0
#define NUM_7_LOW_Y 410
#define NUM_7_HIGH_X 217
#define NUM_7_HIGH_Y 480

#define NUM_8_LOW_X 217
#define NUM_8_LOW_Y 410
#define NUM_8_HIGH_X 434
#define NUM_8_HIGH_Y 480

#define NUM_9_LOW_X 434
#define NUM_9_LOW_Y 410
#define NUM_9_HIGH_X 651
#define NUM_9_HIGH_Y 480

#define NUM_0_LOW_X 651
#define NUM_0_LOW_Y 330
#define NUM_0_HIGH_X 800
#define NUM_0_HIGH_Y 410

#define BACKSPACE_LOW_X 651
#define BACKSPACE_LOW_Y 250
#define BACKSPACE_HIGH_X 800
#define BACKSPACE_HIGH_Y 330

#define ENTER_LOW_X 651
#define ENTER_LOW_Y 410
#define ENTER_HIGH_X 800
#define ENTER_HIGH_Y 480

#define SET_PWD_LOW_X 620
#define SET_PWD_LOW_Y 0
#define SET_PWD_HIGH_X 800
#define SET_PWD_HIGH_Y 80

#define EXIT_PWD_LOW_X 0
#define EXIT_PWD_LOW_Y 0
#define EXIT_PWD_HIGH_X 65
#define EXIT_PWD_HIGH_Y 65

int getInput(int pwdLength, char *pwd);
void getInputPwd(int *pwdLength, char *pwd, char num);
int verifyPwd(int pwdLength, char *pwd);
void setPassword(int pwdLength, char *pwd);
/**
 * 1.进入密码界面
 * 2.获取输入
 * 3.回车后，校验密码
 * 4.输入密码后，可以设置密码将密码保存到密码文件
 * 
 * 返回值
 * 0：退出
 * 1：密码正确进入程序
*/
int password()
{
    //显示密码界面
    draw_bmp_offset("/kjh/RESOURCES/password.bmp", 0, 0);
    draw_bmp_offset("/kjh/RESOURCES/lock.bmp", 345, 40);
    draw_bmp_offset("/kjh/RESOURCES/exit.bmp", 0, 0);
    char pwd[8];
    int pwdLength = 0;
    int x, y;              //获取点击坐标
    int verifyReturn = -1; //密码验证结果,1：密码正确；0：密码错误；-1：读取密码文件失败
    open_tsfile();
    while (1)
    {
        get_xy(&x, &y);
        if (x > NUM_1_LOW_X && x < NUM_1_HIGH_X)
        {
            if (y > NUM_1_LOW_Y && y < NUM_1_HIGH_Y)
            {
                printf("1\n");
                getInputPwd(&pwdLength, pwd, '1');
            }
            if (y > NUM_4_LOW_Y && y < NUM_4_HIGH_Y)
            {
                printf("4\n");
                getInputPwd(&pwdLength, pwd, '4');
            }
            if (y > NUM_7_LOW_Y && y < NUM_7_HIGH_Y)
            {
                printf("7\n");
                getInputPwd(&pwdLength, pwd, '7');
            }
        }
        if (x > NUM_2_LOW_X && x < NUM_2_HIGH_X)
        {
            if (y > NUM_2_LOW_Y && y < NUM_2_HIGH_Y)
            {
                printf("2\n");
                getInputPwd(&pwdLength, pwd, '2');
            }
            if (y > NUM_5_LOW_Y && y < NUM_5_HIGH_Y)
            {
                printf("5\n");
                getInputPwd(&pwdLength, pwd, '5');
            }
            if (y > NUM_8_LOW_Y && y < NUM_8_HIGH_Y)
            {
                printf("8\n");
                getInputPwd(&pwdLength, pwd, '8');
            }
        }
        if (x > NUM_3_LOW_X && x < NUM_3_HIGH_X)
        {
            if (y > NUM_3_LOW_Y && y < NUM_3_HIGH_Y)
            {
                printf("3\n");
                getInputPwd(&pwdLength, pwd, '3');
            }
            if (y > NUM_6_LOW_Y && y < NUM_6_HIGH_Y)
            {
                printf("6\n");
                getInputPwd(&pwdLength, pwd, '6');
            }
            if (y > NUM_9_LOW_Y && y < NUM_9_HIGH_Y)
            {
                printf("9\n");
                getInputPwd(&pwdLength, pwd, '9');
            }
        }
        if (x > NUM_0_LOW_X && x < NUM_0_HIGH_X)
        {
            if (y > NUM_0_LOW_Y && y < NUM_0_HIGH_Y)
            {
                printf("0\n");
                getInputPwd(&pwdLength, pwd, '0');
            }
            if (y > BACKSPACE_LOW_Y && y < BACKSPACE_HIGH_Y)
            {
                printf("backspace\n");
                if (pwdLength != 0)
                {
                    draw_bmp_offset("/kjh/RESOURCES/white.bmp", 20 + 65 * (pwdLength - 1), 185);
                    pwdLength--;
                }
            }
            if (y > ENTER_LOW_Y && y < ENTER_HIGH_Y)
            {
                printf("enter\n");
                verifyReturn = verifyPwd(pwdLength, pwd);
                printf("verify result:");
                printf("%d\n", verifyReturn);
                if (verifyReturn == 0)
                {
                    draw_bmp_offset("/kjh/RESOURCES/pwd_wrong.bmp", 20, 130);
                }
                if (verifyReturn == 1)
                {
                    draw_bmp_offset("/kjh/RESOURCES/unlock.bmp", 342, 22);
                    draw_bmp_offset("/kjh/RESOURCES/white.bmp", 0, 100);
                    draw_bmp_offset("/kjh/RESOURCES/white.bmp", 50, 100);
                    sleep(1);
                    close_tsfile();
                    return 1;
                }
                //pwdLength归零
                if (pwdLength != 0)
                {
                    while (1)
                    {
                        draw_bmp_offset("/kjh/RESOURCES/white.bmp", 20 + 65 * (pwdLength - 1), 185);
                        if (--pwdLength == 0)
                        {
                            break;
                        }
                    }
                }
                printf("pwdLength:");
                printf("%d\n", pwdLength);
            }
        }
        if (x > SET_PWD_LOW_X && x < SET_PWD_HIGH_X && y > SET_PWD_LOW_Y && y < SET_PWD_HIGH_Y)
        {
            printf("set password");
            setPassword(pwdLength, pwd);
            if (pwdLength != 0)
            {
                while (1)
                {
                    draw_bmp_offset("/kjh/RESOURCES/white.bmp", 20 + 65 * (pwdLength - 1), 185);
                    if (--pwdLength == 0)
                    {
                        break;
                    }
                }
            }
        }
        if (x > EXIT_PWD_LOW_X && x < EXIT_PWD_HIGH_X && y > EXIT_PWD_LOW_Y && y < EXIT_PWD_HIGH_Y)
        {
            close_tsfile();
            return 0;
        }
    }
    close_tsfile();
    return 0;
}

void setPassword(int pwdLength, char *pwd)
{
    if (pwdLength == 0)
    {
        return;
    }
    int pfile = open("/kjh/RESOURCES/password.txt", O_RDWR, O_CREAT);
    lseek(pfile, 0, SEEK_END);
    write(pfile, pwd, pwdLength);
    lseek(pfile, 0, SEEK_END);
    char *gan = "|";
    write(pfile, gan, 1);
    close(pfile);
}

void getInputPwd(int *pwdLength, char *pwd, char num)
{
    if (*pwdLength == 8)
    {
        return;
    }
    pwd[*pwdLength] = num;
    draw_bmp_offset("/kjh/RESOURCES/xing.bmp", 20 + 65 * (*pwdLength), 185);
    (*pwdLength)++;
}

int verifyPwd(int pwdLength, char *pwd)
{
    if (pwdLength == 0)
    {
        return 0;
    }
    int result = -1;           //result=1:密码正确； result=0:密码错误；result=-1:读取密码文件失败
    int i = 0;                 //用作数组下标
    char pwdFromFile[9];       //密码默认最多8位，第九位是换行符
    int pwdFromFileLength = 0; //读取出来的密码位数,包括换行符
    int readLength = 0;
    //读取密码文件
    int pwdFile = open("/kjh/RESOURCES/password.txt", O_RDONLY);
    if (pwdFile == -1)
    {
        close(pwdFile);
        return result; //读取密码文件失败
    }
    while (1)
    {
        //读取密码文件中的密码。换行符分隔密码。
        do
        {
            readLength = read(pwdFile, pwdFromFile + pwdFromFileLength, 1);
            if (readLength == 0 || readLength == -1) //0，到文件末尾
            {
                result = 0;
                close(pwdFile);
                return result;
            }
            pwdFromFileLength++;
        } while (pwdFromFile[pwdFromFileLength - 1] != '|');

        //先比较密码位数
        if (pwdFromFileLength - 1 != pwdLength) //-1是除去换行符
        {
            pwdFromFileLength = 0;
            continue;
        }
        else
        {
            //循环比较两个数组中的字符
            while (i < pwdLength)
            {
                if (pwd[i] != pwdFromFile[i])
                {
                    break;
                }
                i++;
            }
            if (i == pwdFromFileLength - 1)
            {
                result = 1; //密码正确
                close(pwdFile);
                return result;
            }
            else
            {
                i = 0;
                pwdFromFileLength = 0;
                continue;
            }
        }
    }
}