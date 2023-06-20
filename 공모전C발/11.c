#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#pragma warning(disable:4996) //옛날 함수 경고 없이 사용 

/* -------------------------------------- 열거형 ---------------------------------------------- */

//인물 관련 열거형
typedef enum _job { BAEKSU, ADMIN, PRIEST, DOCTOR, BANKER, MERCH } jobType; //직업 기본상태=NONE
typedef enum _estate { COMMONS, NOBILITY } estate; //신분
typedef enum _scholarship { BABO, THEOLOGY, LAW, MEDICINE } scholarship; //학문 기본상태=NONE


/* -------------------------------------- 구조체 ---------------------------------------------- */

typedef struct _city //도시 정보
{
	char name[10]; //도시 이름
	int popularity; //인구수 
	int building[5]; //지어진 건물.  1. 대학교 2. 은행 3. 성당 4. 미술관

	int workman[99]; //들어간 사람(인덱스로 구분)
} city;

typedef struct _person //인물 정보
{
	char name[20]; //이름
	int age; //나이
	int gender; //성별. 1이 남자 2가 여자 랜덤으로 성별 정함 ㅇㅇ
	jobType job; //직업종류
	estate status;//신분종류
	scholarship scholar; //학문종류
	int scholarExp; //학문 숙련도 => 처음에 사람마다 6부터 12 숫자 랜덤으로 하나씩 주고 매턴마다 0부터 
	//시작해서 1씩 올리면 될듯
	//예술가 포함    랜덤 최소 6년 최대 12년 => 이러면 숙달함
	int maxExp;//6~12사이랜덤
	int city; //현재 위치한 도시
	int work; //현재 하는 활동. 0번은 명령 대기 1. 도시조사 2. 돈벌기 3. 학문쌓기(대학교가 있을 때만)
	int AP; // Active Point
} person; //인물 구별 하려면 배열 숫자에 의미부여

typedef struct _artist //예술가 정보
{
	char name[50]; //이름
	int startYear;
	int endYear;
	char paintNameA[100];
	int paintYearA;

	char paintNameB[100];
	int paintYearB;

	int city;

	//예술가가 생성될 도시, 연도 그리고 죽을 연도, 작품 2개 남길 연도
} artist; //인물 구별 하려면 배열 숫자에 의미부여

typedef struct _artwork //예술품 정보
{
	char name[100]; //이름
	char painterName[50];

	int year;//예술품 제작 시기

} artwork;

/*--------------------------------------함수----------------------------------------------*///이상한 노래라니 말이 심하네 노래는 다 좋아

void date(person* person, int maxPeople) // 몇일 지나가면 싹다 나이 먹이고 처리하는 함수
{
	int deathDice = 0;
	int deathRate = 0;

	for (int index = 0; index <= maxPeople; index++) //최대 사람수만큼 리스르틑 모조리 스캔
	{
		if (*person[index].name != '\0') // 죽어서 빈 사람이 아닐 경우
		{
			person[index].age++;

			if (person[index].age >= 40) //40세부터 판정 시작.1년 마다 4.5% 그래서 40살에서 60살 되면 100%됨
			{
				deathDice = rand() % 1000;
				deathRate = person[index].age * 45 - 800;

				if (deathRate < 100)
				{
					deathRate = 100;
				}

				if (deathDice > deathRate)//나이별 확률을 주사위가 넘으면
				{
					//나중에 여기 줄에 사망 메시지 출력할 것.
					*person[index].name = '\0';//인물 사망(데이터 초기화) (교수님께 여쭤볼까? 메모메모.)
				}
			}
		}
	}

	return;
}
//이준성 나와 여기 사장 나와

// 첫번째 실행 한정 가문명 정하기 + 
void firstPerson(char* firstname)
{
	printf("성씨(가문명)를 입력해주세요 : ");
	scanf("%s", firstname);

	return;
}

void scholarSelect(scholarship* scholar,jobType* job) //학문 정하기
{
	int select;
	while (true)
	{
		printf("1.신학, 2.법학, 3.의학\n");
		printf("학문을 선택해주세요 : ");//BAEKSU, ADMIN, PRIEST, DOCTOR, BANKER, MERCH
		scanf("%d", &select);

		switch (select) //BABO = 0, THEOLOGY = 1, LAW = 2, MEDICINE = 3
		{
		case 1:
		{
			*scholar = THEOLOGY;
			*job = PRIEST;
			return;
		}

		case 2:
		{
			*scholar = LAW;
			*job = ADMIN;
			return;
		}
		case 3:
		{
			*scholar = MEDICINE;
			*job = DOCTOR;
			return;
		}
		default:
		{
			*scholar = BABO;
			*job = BAEKSU;
			return;
		}
		}
	}
}

// 이름 정할때마다 쓰게될 함수
void naming(person* name, int index)
{
	printf("이름을 입력해주세요 : ");
	scanf("%s", name[index].name);

	return;
}

// E_도시투자 중 건물 건설
void build(city* city, int cityindex,int* money, int* honor)//아아 ㅇㅋ
{
	while (true)
	{
		int select = 0;
		int buildSelect;
		printf("도시 건물 정보를 보시려면 1번을 선택해주세요.\n바로 건물을 지을 거라면 다른 번호를 선택해주세요 : ");
		scanf("%d", &select);

		if (select == 1)
		{
			printf("도시 이름 : %s\n", city[cityindex].name);
			printf("존재하는 건물 목록 : ");

			if (city[cityindex].building[1] == 1) printf("대학교, ");
			if (city[cityindex].building[2] == 1) printf("은행, ");
			if (city[cityindex].building[3] == 1) printf("성당, ");
			if (city[cityindex].building[4] == 1) printf("미술관, ");
			printf("\b\b");
			printf("\n");
		}


		while (1)
		{
			char select = 0;
			printf("1.대학교 2.은행 3.성당 4.미술관 5.취소\n");//지워지워 다 지워버려 그냥 다 지워ㅓㅓㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
			printf("지을 건물을 선택해주세요 (숫자): ");
			scanf("%d", &buildSelect);
			if (buildSelect != 5)
			{
				if (city[cityindex].building[buildSelect] == 0)
				{
					switch(buildSelect)
					{
					case 1:
						printf("대학교를 지을려면 10000G가 필요합니다. 지으시겠습니까? (Y/N)");
			
						scanf(" %c", &select);
						if (select == 'Y')
						{
							if (*money > 10000)
							{
								city[cityindex].building[buildSelect] = 1;
								*honor += 1;
								printf("완료되었습니다.\n");
								return;
							}
							else
							{
								printf("돈이 부족합니다.\n");
								continue;
							}
						}
						else
						{
							continue;
						}
					case 2:
						printf("은행을 지을려면 15000G가 필요합니다. 지으시겠습니까? (Y/N)");
						scanf(" %c", select);
						if (select == 'Y')
						{
							if (*money > 15000)
							{
								city[cityindex].building[buildSelect] = 1;
								*honor += 1;
								printf("완료되었습니다.\n");
								return;
							}
							else
							{
								printf("돈이 부족합니다.\n");
								continue;
							}
						}
						else
						{
							continue;
						}
					case 3:
						printf("성당을 지을려면 20000G가 필요합니다. 지으시겠습니까? (Y/N)");
						scanf(" %c", select);
						if (select == 'Y')
						{
							if (*money > 20000)
							{
								city[cityindex].building[buildSelect] = 1;
								*honor += 2;
								printf("완료되었습니다.\n");
								return;
							}
							else
							{
								printf("돈이 부족합니다.\n");
								continue;
							}
						}
						else
						{
							continue;
						}
					case 4:
						printf("미술관을 지을려면 15000G가 필요합니다. 지으시겠습니까? (Y/N)");
						scanf(" %c", select);
						if (select == 'Y')
						{
							if (*money > 15000)
							{
								city[cityindex].building[buildSelect] = 1;
								*honor += 2;
								printf("완료되었습니다.\n");
								return;
							}
							else
							{
								printf("돈이 부족합니다.\n");
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					
				}
				else
				{
					printf("이미 지어진 건물입니다.\n");
				}
			}
			else
			{
				printf("도시 건설을 취소합니다.\n");
				return;
			}
		}
	}
	return;
}

//도시 보내기
void movePeople(int* cityMove)
{
	printf("이동을 원하는 도시를 선택하세요: ");
	scanf("%d", cityMove);
	printf("해당 인물을 %d번 도시로 이동했습니다.\n", *cityMove);
	return;
}

//건물 집어넣기
/*void movePeople(int* placeMove)
{
	while (1)
	{
		printf("이동을 원하는 도시를 선택하세요: ");
		scanf("%d",placeMove);
		if (1 <= *placeMove <= 6)
		{
			printf("해당 인물을 %d번 건물로 이동했습니다.", *placeMove);
		}
	}
}*/

//인물조사.
void searching(int searchCity, person* newFace)
{
	person newPerson;
	int random;
	char select;
	char nameList[300][30] = { "Alessandro", "Aurora", "Marco", "Sofia", "Lorenzo",
	"Valentina", "Matteo", "Giorgia", "Andrea", "Chiara", "Francesco",
	"Beatrice", "Davide", "Martina", "Giuseppe", "Alice", "Leonardo",
	"Aurora", "Filippo", "Laura", "Mattia", "Sara", "Simone", "Giulia",
	"Gabriele", "Valeria", "Luca", "Giulia", "Mattia", "Elena", "Giacomo",
	"Aurora", "Alessandro", "Martina", "Riccardo", "Caterina", "Giovanni",
	"Maria", "Alessandro", "Elena", "Pietro", "Chiara", "Francesco", "Sofia",
	"Luca", "Laura", "Matteo", "Valentina", "Marco", "Aurora", "Giuseppe", "Alice",
	"Andrea", "Sofia", "Leonardo", "Martina", "Lorenzo", "Beatrice", "Davide", "Giulia",
	"Alessandro", "Emma", "Gabriele", "Valeria", "Simone", "Francesca", "Filippo", "Laura",
	"Matteo", "Sara", "Giovanni", "Sofia", "Marco", "Martina", "Alessia", "Francesco",
	"Aurora", "Leonardo", "Chiara", "Andrea", "Valentina", "Matteo", "Emma", "Lorenzo",
	"Beatrice", "Davide", "Giorgia", "Alessandro", "Sara", "Simone", "Giulia", "Gabriele",
	"Martina", "Matteo", "Valentina", "Francesco", "Aurora", "Luca", "Chiara", "Andrea",
	"Sofia", "Giovanni", "Francesca", "Lorenzo", "Beatrice", "Davide", "Valentina", "Matteo",
	"Emma", "Alessandro", "Sara", "Simone", "Giulia", "Gabriele", "Martina", "Francesco",
	"Aurora", "Luca", "Chiara", "Andrea", "Sofia", "Giovanni", "Francesca", "Lorenzo",
	"Beatrice", "Davide", "Valentina", "Matteo", "Emma", "Alessandro", "Sara", "Simone",
	"Giulia", "Gabriele", "Martina", "Francesco", "Aurora", "Luca", "Chiara", "Andrea",
	"Sofia", "Giovanni", "Francesca", "Lorenzo", "Beatrice", "Davide", "Valentina", "Matteo",
	"Emma", "Alessandro", "Sara", "Simone", "Giulia", "Gabriele", "Martina", "Francesco",
	"Aurora", "Luca", "Chiara", "Andrea", "Sofia", "Giovanni", "Francesca", "Lorenzo",
	"Beatrice", "Davide", "Valentina", "Matteo", "Emma", "Alessandro", "Sara", "Simone",
	"Giulia", "Gabriele", "Martina", "Francesco", "Aurora", "Luca", "Chiara", "Andrea",
	"Sofia", "Giovanni", "Francesca", "Lorenzo", "Beatrice", "Davide", "Valentina",
	"Matteo", "Emma", "Alessandro", "Sara", "Simone", "Giulia", "Gabriele", "Martina",
	"Francesco", "Aurora", "Luca", "Chiara", "Andrea", "Sofia", "Giovanni", "Francesca",
	"Lorenzo", "Beatrice", "Davide", "Valentina", "Matteo", "Emma", "Alessandro", "Sara",
	"Simone", "Giulia", "Gabriele", "Martina", "Francesco", "Aurora", "Luca", "Chiara",
	"Andrea", "Sofia", "Giovanni", "Francesca", "Lorenzo", "Beatrice", "Davide",
	"Valentina", "Matteo", "Emma", "Alessandro", "Sara", "Simone", "Giulia", "Gabriele",
	"Martina", "Francesco", "Aurora", "Luca", "Chiara", "Andrea", "Sofia", "Giovanni",
	"Francesca", "Lorenzo", "Beatrice", "Davide", "Valentina", "Matteo", "Emma",
	"Alessandro", "Sara", "Simone", "Giulia", "Gabriele", "Martina", "Francesco",
	"Aurora", "Luca", "Chiara", "Andrea" };

	printf("%d번 도시에서 새로운 인물을 찾았습니다!\n", searchCity);

	srand((unsigned int)time(NULL));
	random = rand() % 5;
	strcpy(newPerson.name, nameList[random]);
	printf("이름: %s\n", newPerson.name);

	srand((unsigned int)time(NULL));
	newPerson.age = rand() % 9 + 20;
	printf("나이: %d\n", newPerson.age);

	srand((unsigned int)time(NULL));
	newPerson.gender = rand() % 2 + 1;
	printf("성별: %c\n", newPerson.gender == 1 ? 'M' : 'F');

	srand((unsigned int)time(NULL));
	random = rand() % 6;
	printf("직업: ");
	switch (random)
	{
	case 0://18세미만 또는 대학을 감 
		newPerson.job = BAEKSU;

		printf("무직\n");
		break;

	case 1://학문 직업. 법학
		newPerson.job = ADMIN;

		printf("행정관\n");
		break;

	case 2://학문 직업. 신학
		newPerson.job = PRIEST;

		printf("성직자\n");
		break;

	case 3://학문 직업. 의학
		newPerson.job = DOCTOR;

		printf("의사\n");
		break;

	case 4:
		newPerson.job = BANKER;

		printf("은행가\n");
		break;

	case 5:
		newPerson.job = MERCH;

		printf("상인\n");
		break;

	default:
		printf("오류남 ㅋㅋ\n");
	}


	srand((unsigned int)time(NULL));
	random = rand() % 100;
	printf("신분: ");
	if(random<3)
	{
		newPerson.status = COMMONS;

		printf("평민\n");
	}
	else
	{
		newPerson.status = NOBILITY;

		printf("귀족\n");
	}
	
	srand((unsigned int)time(NULL));
	random = rand() % 4;
	printf("학문: ");
	switch (random)
	{
	case 0:
		newPerson.scholar = BABO;
		printf("배우지 않음\n");
		break;
	case 1:
		newPerson.scholar = THEOLOGY;
		printf("신학\n");
		break;
	case 2:
		newPerson.scholar = LAW;
		printf("법학\n");
		break;
	case 3:
		newPerson.scholar = MEDICINE;
		printf("의학\n");
		break;
	default:
		printf("여기 오류남 ㅋㅋㅋㅋ\n\n");
	}

	srand((unsigned int)time(NULL));
	newPerson.scholarExp = 0;
	if (newPerson.scholar != BABO)
	{
		srand((unsigned int)time(NULL));
		newPerson.maxExp = rand() % 6 + 6;
	}
	else newPerson.maxExp = 0;

	newPerson.city = searchCity;
	newPerson.work = 0;
	newPerson.AP = 1;

	*newFace = newPerson;
	printf("조사한 인물 정보가 추가되었습니다.\n\n");
	return;
}

//가문 인물 목록 기능
void showPersonList(person* person, int maxPerson, char firstName[10], int searchCheck)
{
	for (int index = 0; index < maxPerson; index++)
	{
		if (person[index].name != '\0')
		{
			if(searchCheck == 0)
				printf("%d. %s %s\n", index + 1, firstName, person[index].name); // 0이 아니고 1부터 번호 매겨서 인물 출력하고 나중에 입력 받을때 -1 하기
			else
				printf("%d. %s\n", index + 1, person[index].name);
			
			printf("나이: %d\n", person[index].age);
			printf("성별: %c\n", person[index].gender == 1 ? 'M' : 'F');
			printf("AP: %d\n", person[index].AP);


			printf("직업: ");
			switch (person[index].job)
			{
			case BAEKSU://18세미만 또는 대학을 감 
				printf("무직\n");
				break;
			case ADMIN://학문 직업. 법학
				printf("행정관\n");
				break;
			case PRIEST://학문 직업. 신학
				printf("성직자\n");
				break;
			case DOCTOR://학문 직업. 의학
				printf("의사\n");
				break;
			case BANKER:
				printf("은행가\n");
				break;
			case MERCH://바로 고를 수 있는 직업
				printf("상인\n");
				break;
			default:
				printf("오류남 ㅋㅋ\n");
			}


			printf("신분: ");
			switch (person[index].status)
			{
			case COMMONS:
				printf("평민\n");
				break;
			case NOBILITY:
				printf("귀족\n");
				break;
			default:
				printf("님들 여기 오류남!!\n");
			}


			printf("학문: ");
			switch (person[index].scholar)
			{
			case BABO:
				printf("배우지 않음\n");
				break;
			case THEOLOGY:
				printf("신학\n");
				break;
			case LAW:
				printf("법학\n");
				break;
			case MEDICINE:
				printf("의학\n");
				break;
			default:
				printf("여기 오류남 ㅋㅋㅋㅋ\n");
			}

			if(person[index].city != 0 && searchCheck == 0)
			{
				printf("현재 %d번 도시에 위치해 있습니다.\n", person[index].city);
			}
			else
			{
				printf("현재 도시에 발령되지 않은 인물입니다.\n");
			}

			if(searchCheck == 0)
			{
				switch(person[index].work)//현재 하는 활동. 0. 명령 대기 1. 도시조사 2. 돈벌기 3. 학문쌓기(대학교가 있을 때만)
				{
					case 1:
						printf("도시를 조사하고 있습니다.\n");
						break;
					case 2:
						printf("돈을 버는 중입니다.\n");
						break;
					case 3:
						printf("대학교에서 공부하는 중입니다.\n");
						break;
					case 0:
						printf("명령을 기다리는 중입니다.\n");
						break;
					default:
						break;
				}
			}

			if (person[index].scholar != BABO && searchCheck == 0)
			{
				if (person[index].scholarExp != person[index].maxExp)
					printf("학문을 더 습득할 필요가 있습니다.\n");
			}
		}
		printf("\n");
	}
	return;
}


void printMap(int year, int money, int honor)
{
	printf("%d년 | 돈__%d | 명성__%d |\n", year, money, honor);
	printf("==================================================================================================== \n");
	printf("                    /                               _______ /        \n");
	printf("1.런던(항)         /                ---------------                  \n");
	printf("------------------                /  2.브뤼헤(항)\033[33m_______\n\033[0m");
	printf("              ------------------    \033[33m/                 _ \033[0m4.바젤     \n\033[0m");
	printf("            /                      \033[33m/              ___/             \n\033[0m");
	printf("-----------                3.파리\033[33m__________    __/    \033[32m^^^^^^^^^^^^^^ \n\033[0m");
	printf("                           \033[33m＼             \033[0m5.제네바    \033[32m^^^^^^^^^^^^^ \n\033[0m");
	printf("                             \033[33m＼             _|_   \033[32m^^^^^^^^^   \033[33m________ \n\033[0m");
	printf("                            6.리옹         \033[33m|   ＼     \033[32m^^^^^  \033[33m/   \033[0m_____\033[33m＼\033[0m__ \n\033[0m");
	printf("                              \033[32m^^^^^^^^^^^ \033[33m/     |         __/ \033[0m__/    9.베네치아(항) \n\033[0m");
	printf("                      \033[32m^^^^^^^^^^ \033[33m________/      ＼ ____ /  \033[0m__/                 ＼ \n\033[0m");
	printf("             \033[32m^^^^^^^^^^^^   \033[33m___/               \033[0m10.밀라노  /                      ＼ \n\033[0m");
	printf("              \033[32m^^^^^^^^^   \033[33m/                      /       \033[0m/                         ＼ \n\033[0m");
	printf("\033[32m^^                \033[0m7.제노바(항)\033[33m__________________|        \033[0m＼                          | \n\033[0m");
	printf("\033[32m^^^^^^            \033[0m/           ＼                \033[33m＼\033[0m11.피렌체＼                         ＼ \n\033[0m");
	printf("   \033[32m^^^^^^^^    \033[0m/                ＼           \033[33m_____/     |    \033[0m|                          |   \n\033[0m");
	printf("             -                    ＼   12.피사          \033[33m|    \033[0m|                          |   \n");
	printf("           /                        ＼       \033[33m|           ----\033[0m13.안코나(항)              | \n");
	printf("   8.바르셀로나(항)                   ＼     \033[33m|              /  \033[0m＼                        ＼ \n");
	printf("    /                                   ＼   14.로마\033[33m______ /    \033[0m ＼                        | \n");
	printf("----                                      ＼ ___     \033[33m＼            \033[0m＼                     / \n");
	printf("                                                 ＼    \033[33m＼            \033[0m＼                  / \n");
	printf("                                                   ＼    \033[33m＼            \033[0m＼                ＼  \n");
	printf("                                                      15.나폴리(항)       ＼               ＼ \n");
	printf("                                                     /          / ＼       |                 ＼ \n");
	printf("                                                    /__________/    ＼_____|                   ＼ \n");
	printf("==================================================================================================== \n");
	return;
}

int revenue(jobType job,estate nobility)//NOBILITY
{
	int random;
	srand((unsigned int)time(NULL));
	
	switch (job) //BAEKSU, ADMIN, PRIEST, DOCTOR, BANKER, MERCH
	{
	case BAEKSU:
		random = rand() % 10 / 10 * 10 + 50;
		if (nobility == 1)
		{
			random * 2;
		}
		printf("구걸로 %dG를 벌었습니다.\n",random);
		return random;
	case DOCTOR:
		random = rand() % 1200 / 10 * 10 + 150;
		if (nobility == 1)
		{
			random * 2;
		}
		printf("사람을 치료하고 대금을 받아서 %dG를 벌었습니다.\n",random);
		return random;
	case ADMIN:
		random = rand() % 1300 / 10 * 10 + 200;
		if (nobility == 1)
		{
			random * 2;
		}
		printf("연봉을 받아서 %dG를 벌었습니다.\n",random);
		return random;
	case PRIEST:
		random = rand() % 1500 / 10 * 10 + 250;
		if (nobility == 1)
		{
			random * 2;
		}
		printf("성금을 받아서 %dG를 벌었습니다.\n",random);
		return random;
	case BANKER:
		random = rand() % 2000 / 10 * 10 + 300;
		if (nobility == 1)
		{
			random * 2;
		}
		printf("연봉으로 %dG를 벌었습니다.\n", random);
		return random;
	case MERCH:
		random = rand() % 2500 / 10 * 10 + 350;
		if (nobility == 1)
		{
			random * 2;
		}
		printf("연봉으로 %dG를 벌었습니다.\n", random);
		return random;
	}
	return 0;
}

/*void inputJob(person* person)
{
	int temp = 0;
	int tempJob = 0;
	temp = person->scholar;
	srand((unsigned int)time(NULL));
	do
	{
		tempJob = rand() % 8;
	} while (tempJob == 0);
	person->job = tempJob;
	return;
}*/

int main()
{
	int year = 1390;
	int honor = 0;
	char firstName[10] = { '\0', };

	city cityList[15] = { '\0', };
	strcpy(cityList[0].name, "런던");
	strcpy(cityList[1].name, "브뤼헤");
	strcpy(cityList[2].name, "파리");
	strcpy(cityList[3].name, "바젤");
	strcpy(cityList[4].name, "제네바");
	strcpy(cityList[5].name, "리옹");
	strcpy(cityList[6].name, "제노바");
	strcpy(cityList[7].name, "바르셀로나");
	strcpy(cityList[8].name, "베네치아");
	strcpy(cityList[9].name, "밀라노");
	strcpy(cityList[10].name, "피렌체");
	strcpy(cityList[11].name, "피사");
	strcpy(cityList[12].name, "안코나");
	strcpy(cityList[13].name, "로마");
	strcpy(cityList[14].name, "나폴리");

	artist artistList[31] = {
		{"마사치오", 1401, 1428, "성 삼위일체", 1428, "성전세", 1425, 11},
		{"산드로 보티첼리", 1445, 1510, "봄", 1482, "비너스의 탄생", 1486, 11},
		{"레오나르도 다 빈치", 1452, 1519, "모나리자", 1506, "최후의 만찬", 1498, 11},
		{"미켈란젤로 부오나로티", 1475, 1564, "천지 창조", 1482, "다비드", 1504, 11},
		{"조르조네", 1478, 1510, "폭풍", 1508, "잠자는 비너스", 1510, 9},
		{"티치아노 베첼리오", 1490, 1576, "우르비노 비너스", 1538, "피에타", 1576, 9},
		{"라파엘로", 1483, 1520, "아테네 학당", 1510, "그리소도의 변용", 1520, 13},
		{"한스 홀바인", 1497, 1543, "대사들", 1533, "헨리 8세의 초상화", 1537, 4},
		{"알브레히트 뒤러", 1471, 1528, "모피 코트를 입은 자화상", 1500, "기도하는 손", 1508, 4},
		{"히에로니무스 보스", 1450, 1516, "세속적인 쾌락의 동산", 1510, "일곱 가지 대죄와 네 가지 종말", 1500, 2},
		{"피터르 브뤼헐 더 아우더", 1527, 1569, "네덜란드 속담", 1559, "농가의 혼례", 1568, 2},
		{"엘 그레코", 1541, 1614, "오르가스 백작의 매장", 1588, "톨레도 풍경", 1599, 8},
		{"미켈란젤로 메리시 다 카라바조", 1571, 1610, "홀로페르네스의 목을 베는 유딧", 1599, "골리앗의 머리를 들고 있는 다윗", 1610, 8},
		{"페테르 파울 루벤스", 1577, 1640, "십자가를 세움", 1611, "하마와 악어 사냥", 1616, 5},
		{"렘브란트 하르먼손 반 레인", 1606, 1669, "갈릴래아 호수의 폭풍과 그리스도", 1633, "야경", 1642, 6},
		{"아르테미시아 젠틸레스키", 1596, 1652, "수산나와 두 장로", 1593, 1642, "유디트와 하녀", 1613,  3},
		{"디에고 벨라스케스", 1599, 1660, "브레다 성의 항복", 1635, "시녀들", 1656, 8},
		{"장바티스트시메옹 샤르댕", 1699, 1779, "붉은 가오리", 1728, "아침 기도", 1735, 3},
		{"앙투안 바토", 1684, 1721, "키테라 섬으로의 출항", 1717, "파리스의 심판", 1718, 3},
		{"장프랑수아 밀레", 1814, 1875, "이삭 줍는 여인", 1857, "만종", 1859, 3},
		{"폴 세잔", 1839, 1906, "붉은 조끼를 입은 소년", 1889, "사과와 오렌지", 1900, 6},
		{"폴 고갱", 1848, 1903, "타히티의 여인들", 1891, "우리는 어디에서 왔는가? 우리는 누구인가? 우리는 어디로 가는가?", 1898, 6},
		{"빈센트 반 고흐", 1853, 1890, "별이 빛나는 밤", 1889, "해바라기", 1889, 2},
		{"에두아르 마네", 1832, 1883, "풀밭 위의 점심 식사", 1863, "피리 부는 소년", 1866, 3},
		{"클로드 모네", 1840, 1926, "인상, 해돋이", 1872, "수련", 1920, 3},
		{"앙리 마티스", 1869, 1954, "루마니아 풍의 블라우스를 입은 여인", 1940, "이카루스", 1944, 6},
		{"바실리 칸딘스키", 1866, 1944, "구성 Ⅶ", 1913, "원 속의 원", 1923, 6},
		{"에드바르드 뭉크", 1863, 1944, "절규", 1893, "불안", 1896, 5},
		{"파블로 피카소", 1881, 1973, "아비뇽 처녀들", 1907, "게르니카", 1937, 8},
		{"마르셀 뒤샹", 1887, 1968, "계단을 내려오는 누드", 1912, "샘", 1917, 6},
		{"살바도르 달리", 1904, 1989, "기억의 지속", 1931, "네로의 코 주위의 탈물질화", 1947, 8}
	};
	artist recruitedArtistList[31] = { '\0', };
	int maxArtist = 0;

	artwork artworkList[70] = { '\0', };
	int maxArtwork = 0;

	int money = 100; //돈 <3

	person personList[1000] = { '\0', }; //'\0' 사람 존재 X 의미 , 배열 0이 첫번째 사람
	int maxPerson = 1;
	person searchList[1000] = { '\0', }; //조사해서 발견한 사람 목록.
	int maxSearchPerson = 0;
	int personindex = 0; // 사람 정할때 personindex 사용하기 (함수 투입용,배열 숫자 변수화)
	int order = 0; // 명령 종류 정하기.
	char answer = 0;

	//주인공 셋팅
	while (1) // 처음으로 이름과 가문명 정하기
	{
		firstPerson(&firstName);
		naming(&personList, personindex); // 첫번째 사람 이름 정하기

		printf("당신의 이름은 %s %s (이)가 맞습니까? (Y/N)", firstName, personList[personindex]);
		
		scanf(" %c", &answer); // Y면 진행 N이면 반복

		if (answer == 'Y')
		{
			break;
		}
		else;
	}
	city num = { '\0', };
	int cityindex = 0; // 도시 매치 시켜야됨 (함수 투입용,배열 숫자 변수화)
	//build(&num,cityindex);
	scholarSelect(&personList[0].scholar,&personList[0].job); // 첫인물 학문선택 scholarSelect(&person[personindex].scholar)
	printf("\n");
	personList[0].age = 20;
	personList[0].gender = 1;
	personList[0].scholarExp = 0;
	personList[0].maxExp = 0;
	personList[0].AP = 1;
	personList[0].work = 0;
	int tempCity = 0;
	

	//본게임시작
	while (true) // << 게임 무한루프
	{
		printMap(year, money, honor);
		if (year > 1390)
		{
			int temp = 0;
			for (int index = 0; index < 15; index++)
			{
				if (cityList[index].building[2] == 1)
				{
					temp += 200;
				}
			}
			printf("올해 은행 수입은 %dG입니다.\n", temp);
			money += temp;
		}

		showPersonList(personList, maxPerson, firstName, 0);
		for (int index = 0; index < maxPerson; index++)
		{

			switch (personList[index].work)
			{
				case 0:
					break;
				case 1:
					searching(personList[index].city, &searchList[maxPerson]);
					maxSearchPerson += 1;
					break;
				case 2:
					break;
				case 3:
					break;
			}
		}

		int moneyAnswer;

		//예술가 영입 단계
		for(int index = 0; index < 31; index++)
		{
			if (artistList[index].startYear == year)
			{
				printf("%s가 %d번 도시에 출현했습니다!\n후원금을 주면 우리 가문에 영입할 수 있을지도 모릅니다.\n후원할 금액을 입력하세요(영입을 원하지 않는다면 0) : ", artistList[index].name, artistList[index].city);
				scanf("%d",&moneyAnswer);
				while (1)
				{
					if (0 <= moneyAnswer <= money)
					{
						if (moneyAnswer != 0)
						{
							/*
							typedef struct _artist //예술가 정보
							{
								char name[20]; //이름
								int startYear;
								int endYear;
								char paintNameA[60];
								int paintYearA;

								char paintNameB[60];
								int paintYearB;


								int city;

								예술가가 생성될 도시, 연도 그리고 죽을 연도, 작품 2개 남길 연도
							} artist; //인물 구별 하려면 배열 숫자에 의미부여
							*/
							strcpy(recruitedArtistList[maxArtist].name,artistList[index].name);
							recruitedArtistList[maxArtist].startYear = artistList[index].startYear;
							recruitedArtistList[maxArtist].endYear = artistList[index].endYear;
							strcpy(recruitedArtistList[maxArtist].paintNameA, artistList[index].paintNameA);
							recruitedArtistList[maxArtist].paintYearA = artistList[index].paintYearA;
							strcpy(recruitedArtistList[maxArtist].paintNameB, artistList[index].paintNameB);
							recruitedArtistList[maxArtist].paintYearB = artistList[index].paintYearB;
							recruitedArtistList[maxArtist].city = artistList[index].city;

							maxArtist++;
							money -= moneyAnswer;

							printf("예술가 영입에 성공했습니다!\n\n");
							break;
						}
						else
						{
							printf("예술가 영입을 포기했습니다.\n\n");
							break;
						}
					}
					else
					{
						printf("제대로 된 값을 입력해주세요.\n\n");
					}
				}
			}
		}

		for (int index = 0; index <= maxArtist; index++)
		{
			if (recruitedArtistList[index].paintYearA == year)
			{
				printf("예술가가 새로운 그림을 그렸습니다.\n명성이 증가했습니다!\n\n");
				printf("제목: %s\n", recruitedArtistList[index].paintNameA);
				printf("작가명: %s\n", recruitedArtistList[maxArtist].name);
				printf("제작 년도: %d\n\n", recruitedArtistList[maxArtist].paintYearA);

				strcpy(artworkList[maxArtwork].name, recruitedArtistList[index].paintNameA);
				strcpy(artworkList[maxArtwork].painterName, recruitedArtistList[index].name);
				artworkList[maxArtwork].year = recruitedArtistList[maxArtist].paintYearA;
				
				honor += 4;
				maxArtwork++;
			}
		}

		for (int index = 0; index <= maxArtist; index++)
		{
			if (recruitedArtistList[index].paintYearB == year)
			{
				printf("예술가가 새로운 그림을 그렸습니다.\n명성이 증가했습니다!\n\n");
				printf("제목: %s\n", recruitedArtistList[index].paintNameB);
				printf("작가명: %s\n", recruitedArtistList[maxArtist].name);
				printf("제작 년도: %d\n\n", recruitedArtistList[maxArtist].paintYearB);

				strcpy(artworkList[maxArtwork].name, recruitedArtistList[index].paintNameB);
				strcpy(artworkList[maxArtwork].painterName, recruitedArtistList[index].name);
				artworkList[maxArtwork].year = recruitedArtistList[maxArtist].paintYearB;

				honor += 4;
				maxArtwork++;
			}
		}

		for (int index = 0; index < maxPerson; index++)
		{
			switch (personList[index].work)
			{
			case 0:
				break;
			case 1:
				searching(personList[index].city, &searchList[maxPerson]);
				maxSearchPerson += 1;
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}

		int recruitCheck = 0;
		
		int loop = 1;
		for (int index = 0; index < maxPerson; index++) //AP 초기화
		{
			personList[index].AP = 1;
			personList[index].work = 0;
			if (personList[index].age >= 40)
			{
				if (personList[index].age == 40)
				{
					strcat(personList[index].name, "(사망)");
				}
				personList[index].AP = 0;
			}
		}

		//2단계. 플레이어의 행동 결정
		while (true) //1번 루프가 하루동안 일어나는 일 루프
		{
			printf("현재 %d년 입니다.\n", year);//여기 년도 적기
			fflush(stdin);
			
			while (true)
			{
				int menuselect = 0;
				int searchselect = 0;
				
				printf("1.인물 행동 시키기\n2.인물목록 보기\n3.조사한 인물목록 보기\n4.건물짓기\n5.조사한 인물 영입하기\n6.가지고 있는 예술품 목록 보기\n7.턴 넘기기: ");
				scanf("%d", &menuselect);
				fflush(stdin);
				if (menuselect == 1)
				{
					printf("행동을 내릴 인물을 선택해주세요(숫자만)\n(행동을 내릴 인물이 없다면 0) : "); // 건물 짓는걸 어디다 넣어야될까
					scanf("%d", &personindex);
					fflush(stdin);
					if (personindex == 0)
					{
						break;
					}
					else;
					if (personindex > maxPerson)
					{
						printf("존재하지 않는 인물입니다.");
						continue;
					}
					else
					{
						if (personList[personindex - 1].AP != 1)
						{
							printf("이미 행동을 한 인물입니다.\n");
							continue;
						}
					}
					printf("%s", personList[personindex - 1].name);
					printf("원하는 행동을 입력해주세요.\n1.도시이동\n2.도시 인물 조사\n3.돈벌기\n4.대학입학\n5.예술가후원\n"); //현재 하는 활동. 0. 명령 대기 1. 도시조사 2. 돈벌기 3. 학문쌓기(대학교가 있을 때만)
					scanf("%d", &order);
					fflush(stdin);
					switch (order) //명령 함수들 여기다가 넣기
					{
					case 1:
						movePeople(&personList[personindex - 1].city); //함수에 printf 이미 있음
						break;
					case 2:
						if(personList[personindex - 1].city != 0)
						{
							printf("%d번 인물이 도시에 있는 인물을 조사하기 시작했습니다.\n", personindex); // 조사 함수 안넣음
							personList[personindex - 1].work = 1;
							personList[personindex - 1].AP = 0;
						}
						else
						{
							printf("해당 인물은 도시에 있지 않습니다.\n");
						}
						
						break;
					case 3:
						if(personList[personindex - 1].city != 0)
						{
							printf("%d번 인물이 돈벌이를 시작했습니다.\n", personindex);
							money += revenue(personList[personindex - 1].job, personList[personindex - 1].status);
							//현재 위치한 건물. 0번은 명령 대기 2. 대학교 3. 은행 4. 성당 5. 미술관
							personList[personindex - 1].work = 2;
							personList[personindex - 1].AP = 0;
						}
						else
						{
							printf("해당 인물은 도시에 있지 않습니다.\n");
						}
						
						break;
					case 4:
						tempCity = personList[personindex - 1].city;
						if (cityList[tempCity].building[1] == 1)
						{
							if (money >= 250)
							{
								printf("대학 등록금은 250G입니다. %d번 인물을 대학에 입학시키겠습니까? (Y/N) : ", personindex);
								scanf("%c", &answer);
								if (answer == "Y")
								{
									printf("%d번 인물이 대학에 입학했습니다.\n", personindex);
									money -= 250;
									personList[personindex - 1].work = 3;
									personList[personindex - 1].AP = 0;
								}

							}
							else
							{
								printf("대학 등록금이 부족합니다.\n");
							}
						}
						else
						{
							printf("현 도시에 대학이 존재하지 않습니다.\n");
						}
					case 5:
						
						break;
						// 1번도시이동 2번도시조사 3번도시배치(돈벌기) 4번가문관련(결혼같은거) 5번예술가후원관련 6번학문
					case 6:
						continue;
					default:
						break;
					}
					
				}
				else if (menuselect == 2) // 인물목록
				{
					showPersonList(personList, maxPerson, firstName, 0);
					continue;
				}
				else if (menuselect == 3) // 조사한 인물 목록
				{
					showPersonList(searchList, maxPerson, firstName, 1);
					continue;
				}
				else if (menuselect == 4) // 도시건설
				{
					build(&cityList, cityindex, &money, &honor);
					continue;
				}
				else if (menuselect == 5)//인물 영입하기. (영입 완료) 딱지가 붙으면 영입이 안 되도록.
				{
					while(1)
					{
						if (recruitCheck == 0)
						{
							printf("영입을 원하는 인물을 선택하세요.\n");
							scanf("%d", searchselect);

							if (searchselect < maxSearchPerson)
							{
								if (strchr(searchList[searchselect - 1].name, "(영입 완료)") == NULL)
								{
									personList[maxPerson] = searchList[searchselect - 1]; // 일일이 복사 해야되나?
									printf("%s이(가) 우리 가문에 영입되었습니다.\n", searchList[searchselect - 1].name);
									strcat(searchList[searchselect - 1].name, "(영입 완료)");
									recruitCheck == 1;
								}
								else
								{
									printf("이미 영입한 인물입니다.\n");
								}
							}
							else
							{
								printf("존재하지 않는 인물입니다.\n");
							}
							
						}
						else
							printf("이번 턴에 이미 영입을 했습니다.\n");
							break;
					}
					continue;
				}
				else if (menuselect == 6)//턴넘기기
				{
					for (int index = 0; index <= maxArtwork; index++)
					{
						if (recruitedArtistList[index].paintYearB == year)
						{
							printf("제목: %s\n", artworkList[index].name);
							printf("작가명: %s\n", artworkList[maxArtist].painterName);
							printf("제작 년도: %d\n\n", artworkList[maxArtist].year);
						}
					}
					continue;
				}
				else if (menuselect == 6)//턴넘기기
				{
					loop = 0;
					break;
				}
				
			}
			if (loop == 0)//break 되면 여기 실행
			{
				break;
			}
		}
		if (honor >= 100) //break 되면 여기 실행
		{
			break;
		}
		date(&personList, maxPerson);//날짜 추가     마지막
		printf("다음해로 넘어갑니다.\n\n");
		year++;
	}
	
	return 0;
}

/*
typedef struct _artist //예술가 정보
{
	char name[20]; //이름
	int startYear;
	int endYear;
	char paintNameA[60];
	int paintYearA;

	char paintNameB[60];
	int paintYearB;


	int city;

	예술가가 생성될 도시, 연도 그리고 죽을 연도, 작품 2개 남길 연도
} artist; //인물 구별 하려면 배열 숫자에 의미부여
*/

// 조사한 인물 이름 안나옴(이거 될걸)

// 돈쓰고 명성 올리기(예술가 후원)
// 돈을 벌어서 예술가 후원하고 영입하고
// 예술가는 해당 년도에 해당 도시에 출현함? => 돈박아서 영입경쟁
// 인물 조사 후->인물 중에 그새끼 하나 골라서 플레이어가 한턴 써서 영입 가능 확률임 (명성에 비례해)
// 예술가들이 작품 만들도록 해야함 => 예술가를 영입하면 작품 해당 연도에 예술품을 만듬
// 교황의 명성을 100으로 기준할래, 프랑스왕90, 나폴리왕80, 영국왕70, 아라곤왕60, 밀라노공작50, 브뤼헤백작40