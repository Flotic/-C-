#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <stdbool.h>
#pragma warning(disable:4996) //옛날 함수 경고 없이 사용

/* -------------------------------------- 열거형 ---------------------------------------------- */

//인물 관련 열거형
typedef enum _job { BAEKSU, LORD, KNIGHT, ADMIN, PRIEST, DOCTOR, BANKER, MERCH } jobType; //직업 기본상태=NONE
typedef enum _estate { COMMONS, NOBILITY } estate; //신분
typedef enum _scholarship { BABO, THEOLOGY, LAW, MEDICINE } scholarship; //학문 기본상태=NONE

//도시 관련 열거형
typedef enum _politicalsystem {} politicalSystem; //정치체제

//예술품 관련 열거형
typedef enum _arttype {} artType; //예술품종류

/* -------------------------------------- 구조체 ---------------------------------------------- */

typedef struct _city //도시 정보
{
	char name[10]; //도시 이름 <<< 도시 배열 숫자에 도시 매치 해야될듯
	int popularity; //인구수 
	int building[7]; //지어진 건물. 1. 도서관 2. 대학교 3. 교역소 4. 은행 5. 성당 6. 미술관

	// <<< 그 도시에 0과1로 지어졌냐 안졌냐를 확인해야함, 
	// <<< 밑에 주성이가 만드는 건물 건설 함수랑 연계 필요               흠

	int workman[99]; //들어간 사람(인덱스로 구분)
	politicalSystem politic; //정치체제(열거형으로 구분)
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
	char name[20]; //이름
	int age; //나이
	int gender; //성별. 1이 남자 2가 여자 랜덤으로 성별 정함 ㅇㅇ
	artType type; //만드는 작품 종류
	int artExp; //작업 숙련도 => 높을수록 작품을 가져올 확률이 높아짐
	//시작해서 1씩 올리면 될듯
	//랜덤 최소 6년 최대 12년 => 이러면 숙달함
	int city; //현재 위치한 도시
} artist; //인물 구별 하려면 배열 숫자에 의미부여

typedef struct _artwork //예술품 정보
{
	char name[20]; //이름	
	artType type;//예술품 종류

} artWork;

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

void scholarSelect(scholarship* scholar) //학문 정하기
{
	int select;
	while (true)
	{
		printf("1.신학, 2.법학, 3.의학\n");
		printf("학문을 선택해주세요 : ");
		scanf("%d", &select);

		switch (select) //BABO = 0, THEOLOGY = 1, LAW = 2, MEDICINE = 3
		{
		case 1:
		{
			*scholar = THEOLOGY;
			return;
		}

		case 2:
		{
			*scholar = LAW;
			return;
		}
		case 3:
		{
			*scholar = MEDICINE;
			return;
		}
		default:
		{
			*scholar = BABO;
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
void build(city* city, int cityindex)//아아 ㅇㅋ
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
			printf("존재하는 도시 목록 : ");

			if (city[cityindex].building[1] == 1) printf("도서관, ");//지어진 건물. 1. 도서관 2. 대학교 3. 교역소 4. 은행 5. 성당 6. 미술관
			if (city[cityindex].building[2] == 1) printf("대학교, ");
			if (city[cityindex].building[3] == 1) printf("교역소, ");
			if (city[cityindex].building[4] == 1) printf("은행, ");
			if (city[cityindex].building[5] == 1) printf("성당, ");
			if (city[cityindex].building[6] == 1) printf("미술관, ");
			printf("\b\b");
			printf("\n");
		}


		while (1)
		{
			printf("1.도서관 2.대학교 3.교역소 4.은행 5.성당 6.미술관 7.취소\n");//지워지워 다 지워버려 그냥 다 지워ㅓㅓㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
			printf("지을 건물을 선택해주세요 : ");
			scanf("%d", &buildSelect);
			if (buildSelect != 7)
			{
				if (city[cityindex].building[buildSelect] == 0)
				{
					city[cityindex].building[buildSelect] = 1;
					printf("완료되었습니다.\n");
					return;
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
void searching(int* searchCity, person* newFace)
{
	person newPerson;
	int random;
	char select;

	printf("%d번 도시에서 새로운 인물을 찾았습니다!", *searchCity);


	//printf("이름: %s\n", newPerson.name);

	newPerson.age = rand() % 9 + 20;
	printf("나이: %d\n", newPerson.age);

	newPerson.gender = rand() % 2 + 1;
	printf("성별: %c\n", newPerson.gender == 1 ? 'M' : 'F');

	random = rand() % 8;
	printf("직업: ");
	switch (random)
	{
	case 0://18세미만 또는 대학을 감 
		newPerson.job = BAEKSU;

		printf("무직\n");
		break;

	case 1://귀족직업
		newPerson.job = LORD;

		printf("영주\n");
		break;

	case 2://귀족직업
		newPerson.job = KNIGHT;

		printf("기사\n");
		break;

	case 3://학문 직업. 법학
		newPerson.job = ADMIN;

		printf("행정관\n");
		break;

	case 4://학문 직업. 신학
		newPerson.job = PRIEST;

		printf("성직자\n");
		break;

	case 5://학문 직업. 의학
		newPerson.job = DOCTOR;

		printf("의사\n");
		break;

	case 6:
		newPerson.job = BANKER;

		printf("은행가\n");
		break;

	case 7://바로 고를 수 있는 직업

		printf("상인\n");
		break;

	default:
		printf("오류남 ㅋㅋ\n");
	}

	random = rand() % 2;

	printf("신분: ");
	switch (random)
	{
	case 0:
		newPerson.status = COMMONS;

		printf("평민\n");
		break;
	case 1:
		newPerson.status = NOBILITY;

		printf("귀족\n");
		break;
	default:
		printf("님들 여기 오류남!!\n");
	}

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
		printf("여기 오류남 ㅋㅋㅋㅋ\n");
	}

	newPerson.scholarExp = 0;
	if (newPerson.scholar != BABO)
	{
		newPerson.maxExp = rand() % 6 + 6;
	}
	else newPerson.maxExp = 0;

	newPerson.city = *searchCity;
	newPerson.work = 0;
	newPerson.AP = 1;

	printf("인물을 영입할까요?(Y/N)");
	scanf("%c", &select);
	if (select == "Y")
	{
		printf("인물이 영입되었습니다.");
		*newFace = newPerson;
	}
	if (select == "N")
	{
		printf("인물 영입을 포기했습니다.");
	}
	return;
}

//가문 인물 목록 기능
void showPersonList(person* person, int maxPerson, char firstName[10])
{
	for (int index = 0; index < maxPerson; index++)
	{
		if (person[index].name != '\0')
		{
			printf("%d. %s %s\n", index + 1, firstName, person[index].name); // 0이 아니고 1부터 번호 매겨서 인물 출력하고 나중에 입력 받을때 -1 하기
			printf("나이: %d\n", person[index].age);
			printf("성별: %c\n", person[index].gender == 1 ? 'M' : 'F');
			printf("AP: %d\n", person[index].AP);


			printf("직업: ");
			switch (person[index].job)
			{
			case BAEKSU://18세미만 또는 대학을 감 
				printf("무직\n");
				break;
			case LORD://귀족직업
				printf("영주\n");
				break;
			case KNIGHT://귀족직업
				printf("기사\n");
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


			if (person[index].scholar != BABO)
			{
				if (person[index].scholarExp != person[index].maxExp)
					printf("학문을 더 습득할 필요가 있습니다.\n");
			}
		}
		printf("\n");
	}
	return;
}

//이거 학문명으로 바까줘 => 직업결정으로 넘어가요
void searchPerson(person* person, int* search) //인물 조사 기능 
{

	*search = 1; // 조사하고 나면 0->1 목록 열리게끔 도시마다 변수 달라야 됨
	return;
}
void printMap(int year, int money, int honor)
{
	printf("""%d년 | 돈_%d | 명성 _%d |\n""", year, money, honor);
	printf("""===========================================================\n""");
	printf("""                          /                               _______ /\n""");
	printf("""1.런던항         /              ---------------\n""");
	printf("""--------  /-------\\ ______ /  2.브뤼헤항________________\n""");
	printf("""        /    ------------------    /                                            _ 4.바젤\n""");
	printf(""" __ /   /                       /                                       ___/\n""");
	printf("""-------                3.파리____________             __/           ^^^^^^^^^^^^^^ \n""");
	printf("""                           \\                            5.제네바    ^^^^^^^^^^^^^\n""");
	printf("""                              \\                           _|_   ^^^^^^^^^           _____\n""");
	printf("""                               6.리옹                 |    \\     ^^^^^          /     ____ \\__\n""");
	printf("""                                      ^^^^^^^^^^  /       |              __/  __/      9.베네치아항\n""");
	printf(""" ^ ^^^^^^^^^__ / \\ ____ /      /              |           \\""");
	printf("""                     ^^^^^^^^^^^^  ___/            10.밀라노     /                |              \\\n""");
	printf("""                      ^^^^^^^^^   /                          /            /                  |                 \\\n""");
	printf("""^^                           7.제노바항_________ |               \\                /                   |""");
	printf("""^^^^^^                /     /    |     \\                     \11.피렌체\           /                      \\\n""");
	printf("""   ^^^^^^^^    /       /         |       \\                 /      \\            \\       /                          |   \n""");
	printf("""                 -      _/              |        \\    12.피사         \\           \\   /                             \\\n""");
	printf("""             /   __/                    |          \\         \\              \\ __13.안코나항                     |\n""");
	printf("""   8.바르셀로나항               |            \\         \\          /               \\   \\_                         \\\n""");
	printf("""      /  \\                                  |               \\__   14.로마               \\     \\_                       |\n""");
	printf("""----        \\                                 |                      \\        \\                   \\      \\_                 /\n""");
	printf("""             /                                  \\_                      \\      \\                   \\        \\            /\n""");
	printf("""____ ---                                          \\_                     \\    |                     \\        \\       /\n""");
	printf("""                                                         \\ ________15.나폴리항           |       /      \\ \n""");
	printf("""                                                                        /  /               /\\           |      |           \\ \n""");
	printf("""                                                                      /  /________/      \\____|    /                \\\n""");
	printf("""=============================================================================\n""");
	return;
}


int main()
{
	int year = 1200;
	int honor = 0;
	char firstName[10] = { '\0', };

	city cityList[15];
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

	int money = 100; //돈 <3

	person personList[1000] = { '\0', }; //'\0' 사람 존재 X 의미 , 배열 0이 첫번째 사람
	int maxPerson = 1;
	person searchList[1000] = { '\0', }; //조사해서 발견한 사람 목록.
	int maxSearchPerson = 0;

	artist artistList[1000] = { '\0', }; //'\0' 사람 존재 X 의미 , 배열 0이 첫번째 사람
	int personindex = 0; // 사람 정할때 personindex 사용하기 (함수 투입용,배열 숫자 변수화)
	int order = 0; // 명령 종류 정하기. 어떤 종류가 있지?

	//주인공 셋팅
	while (1) // 처음으로 이름과 가문명 정하기
	{
		firstPerson(&firstName);
		naming(&personList, personindex); // 첫번째 사람 이름 정하기

		printf("당신의 이름은 %s %s (이)가 맞습니까? (Y/N)", firstName, personList[personindex]);
		char answer = 0;
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
	//18세 됐어 직업 or 대학 => 대학선택 => 학문 골라 => 의학 => 수련 연도랜덤인데 만약 12년걸렷어 => 12년됐어 => 그럼 50프로로 의사 나머지 확률로 나머지 직업 확률
	scholarSelect(&personList[0].scholar); // 첫인물 학문선택 scholarSelect(&person[personindex].scholar)
	printf("\n");
	personList[0].age = 20;
	personList[0].gender = 1;
	personList[0].scholarExp = 0;
	personList[0].maxExp = 0;
	personList[0].AP = 1;
	personList[0].work = 0;
	showPersonList(personList, maxPerson, firstName);

	//본게임시작
	while (1) // << 게임 무한루프
	{
		for (int index = 0; index < maxPerson; index++) //AP 초기화
		{
			personList[index].AP = 1;
		}

		//지도 출력이 있어야될듯
		//메뉴 출력이 처음
		printMap(year, money, honor);

		//1단계. 전날 있었던 일들에 대한 설명
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
				;
			case 3:
				;
			}
		}

		//2단계. 플레이어의 행동 결정
		while (true) //1번 루프가 하루동안 일어나는 일 루프고<< date 안쓰려고 그냥 박은거
		{
			printf("현재 %d년 입니다.\n", year);//여기 년도 적기
			fflush(stdin);
			int loop = 1;
			while (loop)
			{
				int menuselect = 0;
				printf("1.인물 행동 시키기\n2.인물목록 보기\n3.건물짓기\n4.턴 넘기기: ");
				scanf("%d", &menuselect);
				if (menuselect == 1)
				{
					printf("행동을 내릴 인물을 선택해주세요(숫자만)\n(행동을 내릴 인물이 없다면 0) : "); // 건물 짓는걸 어디다 넣어야될까
					scanf("%d", &personindex);
					if (personindex == 0)
					{
						break;
					}
					if (personList[personindex - 1].AP != 1)
					{
						printf("이미 행동을 한 인물입니다.\n");
						continue;
					}
					printf("원하는 행동을 입력해주세요.\n1.도시이동\n2.도시조사\n3.도시배치\n4.대학입학\n5.예술가후원\n6.학문\n"); //현재 하는 활동. 0. 명령 대기 1. 도시조사 2. 돈벌기 3. 학문쌓기(대학교가 있을 때만)
					scanf("%d", &order);
					switch (order) //명령 함수들 여기다가 넣기
					{
					case 1:
						movePeople(&personList[personindex - 1].city);
						break;
					case 2:
						printf("%d번 인물이 도시를 조사하기 시작했습니다.\n", personindex);
						personList[personindex - 1].work = 1;
						break;
					case 3:
						printf("%d번 인물이 돈벌이를 시작했습니다.\n", personindex);
						//현재 위치한 건물. 0번은 명령 대기 1. 도서관 2. 대학교 3. 교역소 4. 은행 5. 성당 6. 미술관
						personList[personindex - 1].work = 2;
						break;
					case 4:
						printf("%d번 인물이 대학에 입학했습니다.\n", personindex);
						personList[personindex - 1].work = 3;
						break;
					case 5:
						break;
						// 1번도시이동 2번도시조사 3번도시배치(돈벌기) 4번가문관련(결혼같은거) 5번예술가후원관련 6번학문
					default:
						break;
					}
					personList[personindex - 1].AP = 0;
				}
				else if (menuselect == 2) // 꼽사리
				{
					showPersonList(personList, maxPerson, firstName);
					continue;
				}
				else if (menuselect == 3)
				{
					build(&cityList, cityindex);
					continue;
				}
				else if (menuselect == 4)
				{
					loop = 0;
				}

			}
			if (loop == 0)
			{
				break;
			}
		}
		date(&personList, maxPerson);//날짜 추가     마지막
		printf("다음해로 넘어갑니다.\n\n");
		year++;
	}
	return 0;
}

// 각 건물들 능력 부여해줘야함 이거는 그때그때 판별하면 될듯? 명령어에 달아두자

// 인물조사 기능(사람 턴도 쓰고)
// 도시 조사 변수 각각 만들어야됨 기본값 0으로
// 인물 목록 확인 기능(반복문으로 구조체 출력) => 가문 인물 확인 기능
// (for 문으로 싹 출력하면 됨) << 30분컷 가능
// 
// 가문인물 새기들 도시 이동해야하고
// (가문인물이 위치한 도시 값 변경. 아니면 이동중인 인물은 구조체에 걸리는시간 턴 넣어서 턴 지날때마다 --)
// <<<날짜 턴제 코드만 만들면 꼽사리 껴서 넣으면 끝 
//	인물들이 한턴에 한가지 활동만 할수도록 해줘<< 턴 넘기면 인물들 구조체 상태에 따라서 수입 결정
// 인물 목록 키면 인물 이름 옆에 O나 X 적어줘서 그턴에 활동 했는지 않했는지 알수있게 
// 인물 코드를 잘 연결시켜서 이준성 바보 만들게!
// <<인물들 구조체안에 행동력 넣어놓고 1이 기본 1이면 실행가능 0이면 실행불가능턴지나면 싹다 1로 초기화
// (<<< OX 출력 함수 만들어야됨)(<<<활동 했으면 재조작 못하는 함수로 만들어야됨)
// 명령 바로 가동 결과는(<<<인물들 숫자만큼 계속 함수 사용) 6/13

// 가문인물 새기들 도시배치 만들어야하고
// (턴마다 돈)
// 업무지시는 일시불 땡기는거임 확률임 돈 잃을 수도 있음(은행 필요)(손익 확률)
// <<확률,수익만 따로 정하면 될듯

// 가문인물들 태어나면 이름짓고 나이 먹어감 18살에 대학or직업 선택 대학이면 돈쓰고 좋은 직업 랜덤으로 걸릴 확률 개방
// 가문인물들 도서관 배치 대학배치
// 가문인물들 은행 열리면 은행가 직업 열어주고
// 교역소 지으면 도시에다가 업무지시(손익 확률) 할수있게 기능 열어주고 
// << 상수 0 선언해놨다가 지으면 1로 바꾸기 (도시 구조체에 넣어야될듯) 
// 
// 건물 짓는데 건물 비용 << 이거 생각해야되고 여차저차 돈벌었으면 명성 올리기(예술가 후원)
// 돈을 벌어서 예술가 후원하고 가문대학에 꽂아주고(교육) 영입하고 마 다해야지
// 
// 예술가는 해당 년도에 해당 도시에 출현함
// 다른 명성 높은 왕이나 교황이나 가문이랑 경쟁 가능하도록 => 교황의 명성을 100으로 기준할래, 프랑스왕90, 나폴리왕80, 영국왕70, 아라곤왕60, 밀라노공작50, 브뤼헤백작40

// 인물 조사 후->인물 중에 그새끼 하나 골라서 플레이가 한턴 써서 청혼 가능 확률임 명성에 비례해
// 청혼후 출산 허용 선택임(단어는 그떄가서 바까보고) 출산은 그 가문인물 턴을 사용함 그동안 돈 못 벎 근데 사실 배치 해제 하고 가능한거라 굳이 말안해도 되겠다 
//인물 상태가 뭐냐라고 생각하면 편함 이게 출산 인지 업무인지 배치 인지 이동인지 하나 골라 이거임
// 이혼 기능 추가 좀여 이건 플레이어인물 턴과 해당 인믈 턴 소모함 

// 지도맵 화면출력 명령어입력 아스키코드, 파일 입출력등등 방법 
// 가장 마지막에 렌즈기능 (최후 추가)

// 예쑬가들이 작품 만들도록 해야함(이거 메커니즘 한번 쫙 설명해주라 아님 기획서에 있나) => 작품 나올 시간대에 가장 후원 많이한사람
// 작품 저장한거 확인해야함
// 작품 처음 얻었을때 아스키코드로 그 작품 실제 모습한번 보여줘(파일 입출력 해야겠다)

//(2) 각도시는 매 턴마다(0.035 % ~0.07 %) 사이 숫자로 인구가 증가함.

//(3) 게임의 랜덤성을 위해 위 인구증가확률은 게임시작시 2회 적용시키고 진행한다.

//시벌 이게뭐냐 공원임? 잔디밭이ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ 자꾸 주성이가 코드의 3분의 2라고 들려
//우리코드 푸르게 푸르게 플스5 사야겠다 파판16하고싶다

/* void printMap(int year,int money,int honor)
printf("%d년 | 돈_%d | 명성 _%d |", year, money, honor);
printf("===========================================================");
printf("                          /                               _______ /");
printf("1.런던⚓         /              ---------------");
printf("--------  /-------\\ ______ /  2.브뤼헤⚓________________");
printf("        /    ------------------    /                                            _ 4.바젤");
printf(" __ /   /                       /                                       ___/");
printf("-------                3.파리____________             __/           ^^^^^^^^^^^^^^ ");
printf("                           \\                            5.제네바    ^^^^^^^^^^^^^");
printf("                              \\                           _|_   ^^^^^^^^^           _____");
printf("                               6.리옹                 |    \\     ^^^^^          /     ____ \\__");
printf("                                      ^^^^^^^^^^  /       |              __/  __/      9.베네치아⚓");
printf("                              ^^^^^^^^^^     __/          \\ ____ /      /              |           \\");
printf("                     ^^^^^^^^^^^^  ___/            10.뭐라노     /                |              \\");
printf("                      ^^^^^^^^^   /                          /            /                  |                 \\");
printf("^^                           7.제노바⚓_________ |               \\                /                   |");
printf("^^^^^^                /     /    |     \\                     \11.피렌체\           /                      \\");
printf("   ^^^^^^^^    /       /         |       \\                 /      \\            \\       /                          |   ");
printf("                 -      _/              |        \\    12.피사         \\           \\   /                             \\");
printf("             /   __/                    |          \\         \\              \\ __13.안코나⚓                     |");
printf("   8.바르셀로나⚓               |            \\         \\          /               \\   \\_                         \\");
printf("      /  \\                                  |               \\__   14.로마               \\     \\_                       |");
printf("----        \\                                 |                      \\        \\                   \\      \\_                 /");
printf("             /                                  \\_                      \\      \\                   \\        \\            /");
printf("____ ---                                          \\_                     \\    |                     \\        \\       /");
printf("                                                          \\ ________15.나폴리⚓           |       /      \\ ");
printf("                                                                        /  /               /\\           |      |           \\ ");
printf("                                                                      /  /________/      \\____|    /                \\");
printf("=============================================================================");
printf("Q_도시조사 | W_도시정보 | E _도시투자 | A _인물조사 | S_인물목록 | D_인물교류");
printf("T_1년 뒤   | Y_30일 뒤  | U_60일 뒤   | G_90일 뒤   | Z_예술품목 | X_렌즈활성");
printf("=============================================================================");
printf("입력 창 : ");
return;
*/