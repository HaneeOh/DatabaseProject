#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <mysql.h>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1007"
#define DB_NAME "myDB" // DB 설정

int main(void)
{
	MYSQL *connection = NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_RES *sql_result1;
	MYSQL_RES *sql_result2;
	MYSQL_ROW sql_row;
	int query_stat;
	
	char query[255];

	char entered_id[30];
	char entered_psw1[30];
	char entered_psw2[30];
	char entered_name[30];
	char entered_phone[30];
	int product_id;
	int product_num;
	int order_id = 0;
	int stock = 0;

	int select1;
	int select2;
	char answer[5];
	
	mysql_init(&conn); // DB 초기화
	
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	// DB 연결
	
	mysql_set_character_set(connection, "euckr");

	if (connection)
	{
		query_stat = mysql_query(connection, "select * from member");
		sql_result = mysql_store_result(connection);

		if (sql_result)
		{
			while (TRUE)
			{
				back1:
				printf("1. 로그인 \n");
				printf("2. 회원 가입 \n");
				printf("3. 회원 탈퇴 \n");
				printf("4. 회원 현황 \n");
				printf("0. 종료 \n");
				printf("실행할 번호를 입력해주세요: ");
				scanf_s("%d", &select1);

				if (select1 == 0)
					break;

				// 로그인
				else if (select1 == 1)
				{
					printf("\n");
					printf("	로그인할 회원의 정보를 입력해주세요.\n");
					
					back2:
					printf("	ID: ");
					scanf_s("%s", entered_id, sizeof(entered_id));
					printf("	password: ");
					scanf_s("%s", entered_psw1, sizeof(entered_psw1));

					sprintf_s(query, sizeof(query), "select * from member where m_id = '%s' and m_psw='%s'", entered_id, entered_psw1);
					query_stat = mysql_query(connection, query);
					sql_result = mysql_store_result(connection);

					if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("	==============================로그인 성공!==============================\n");
						printf("\n");

						sprintf_s(query, sizeof(query), "create view personalcart as select p_category, p_name, p_price, c_p_num from cart, product where c_p_id = product.p_id and c_id = '%s'", entered_id);
						query_stat = mysql_query(connection, query);
						sql_result = mysql_store_result(connection);

						back6:
						printf("	===============================상품 확인================================\n");
						printf("	상품번호	분류		   이름		    재고	    가격\n");
						printf("	========================================================================\n");

						sprintf_s(query, sizeof(query), "select p_id, p_category, p_name, p_stock, p_price from product");
						query_stat = mysql_query(connection, query);
						sql_result = mysql_store_result(connection);

						while (sql_row = mysql_fetch_row(sql_result))
						{
							for (int i = 0; i < mysql_num_fields(sql_result); i++)
								printf("	%8s", sql_row[i]);
							printf("\n");
						}

						printf("	========================================================================\n");

						while (TRUE)
						{
							back7:
							printf("\n");
							printf("	1. 장바구니 담기 \n");
							printf("	2. 장바구니 확인 \n");
							printf("	3. 주문하기 \n");
							printf("	4. 주문 확인 \n");
							printf("	5. 비밀번호 수정 \n");
							printf("	6. 로그 아웃 \n");
							printf("	실행할 번호를 입력해주세요: ");
							scanf_s("%d", &select2);
							printf("\n");

							// 장바구니 담기
							if (select2 == 1)
							{
								printf("		장바구니에 담을 상품 번호: ");
								scanf_s("%d", &product_id, sizeof(product_id));
								printf("		수량: ");
								scanf_s("%d", &product_num, sizeof(product_num));

								sprintf_s(query, sizeof(query), "insert into cart values('%s','%d','%d')", entered_id, product_id, product_num);
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);

								printf("		장바구니에 추가되었습니다.\n");
							}
							// 장바구니 확인
							else if (select2 == 2)
							{
								sprintf_s(query, sizeof(query), "select * from personalcart");
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);

								printf("		=====================장바구니 확인=====================\n");
								printf("		상품분류	이름		가격		수량\n");
								printf("		=======================================================\n");

								while (sql_row = mysql_fetch_row(sql_result))
								{
									for (int i = 0; i < mysql_num_fields(sql_result); i++)
										printf("		%5s", sql_row[i]);
									printf("\n");
								}

								printf("		장바구니에 담긴 전체 상품을 주문하시겠습니까? (Y/N) : ");
								scanf_s("%s", answer, sizeof(answer));

								if (strcmp(answer, "Y"))
								{
									goto back7;
								}
								else
								{
									// 주문번호 생성
									sprintf_s(query, sizeof(query), "select * from orderlist");
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);
									
									while (sql_row = mysql_fetch_row(sql_result))
									{
										order_id++;
									}
									order_id += 10;

									// 재고 상황 파악 후 주문 가능한지 확인
									sprintf_s(query, sizeof(query), "select personalcart.p_name, c_p_num, p_stock, p_id from personalcart, product where personalcart.p_name = product.p_name");
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);

									while (sql_row = mysql_fetch_row(sql_result))
									{
										stock = atoi(sql_row[2]);
										product_num = atoi(sql_row[1]);
										product_id = atoi(sql_row[3]);
																
										if (stock >= product_num)
										{
											sprintf_s(query, sizeof(query), "insert into orderlist values('%d', '%s', '%d', '%d')", order_id, entered_id, product_id, product_num);
											query_stat = mysql_query(connection, query);
											sql_result1 = mysql_store_result(connection);

											stock = stock - product_num;

											sprintf_s(query, sizeof(query), "update product set p_stock = '%d' where p_id = '%d'", stock, product_id);
											query_stat = mysql_query(connection, query);
											sql_result1 = mysql_store_result(connection);

											order_id++;

											sprintf_s(query, sizeof(query), "delete from cart where c_id = '%s' and c_p_id = '%d'", entered_id, product_id);
											query_stat = mysql_query(connection, query);
											sql_result1 = mysql_store_result(connection);
										}
									}
									goto back6;
								}
							}
							// 주문하기
							else if (select2 == 3)
							{
								printf("		주문할 상품 번호: ");
								scanf_s("%d", &product_id, sizeof(product_id));
								printf("		수량: ");
								scanf_s("%d", &product_num, sizeof(product_num));

								sprintf_s(query, sizeof(query), "select p_stock from product where p_id='%d'", product_id);
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								
								while (sql_row = mysql_fetch_row(sql_result))
								{
									stock = atoi(sql_row[0]);
								}

								if (stock >= product_num)
								{
									sprintf_s(query, sizeof(query), "select * from orderlist");
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);
;
									while (sql_row = mysql_fetch_row(sql_result))
									{
										order_id++;
									}
									order_id += 10;

									sprintf_s(query, sizeof(query), "insert into orderlist values('%d', '%s', '%d', '%d')", order_id, entered_id, product_id, product_num);
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);

									stock = stock - product_num;

									sprintf_s(query, sizeof(query), "update product set p_stock = '%d' where p_id = '%d'", stock, product_id);
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);

									goto back6;
								}
								else
									printf("		재고 부족으로 주문에 실패했습니다.\n");
							}
							// 주문 확인
							else if (select2 == 4)
							{
								sprintf_s(query, sizeof(query), "select distinct o_id, m_name, p_name, p_price, o_p_num from orderlist, member, product, cart where orderlist.o_m_id = member.m_id and orderlist.o_p_id = product.p_id and o_m_id = '%s'", entered_id);
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);

								printf("		====================================주문 확인===================================\n");
								printf("		주문번호	주문인		상품이름	가격		수량\n");
								printf("		================================================================================\n");

								while (sql_row = mysql_fetch_row(sql_result))
								{
									for (int i = 0; i < mysql_num_fields(sql_result); i++)
										printf("		%5s", sql_row[i]);
									printf("\n");
								}
							}
							// 비밀번호 수정
							else if (select2 == 5)
							{
								back5:
								printf("		변경할 password: ");
								scanf_s("%s", entered_psw1, sizeof(entered_psw1));
								printf("		변경할 password 확인: ");
								scanf_s("%s", entered_psw2, sizeof(entered_psw2));

								if (strcmp(entered_psw1, entered_psw2))
								{
									printf("		비밀번호가 일치하지 않습니다.\n");
									goto back5;
								}
								else
								{
									sprintf_s(query, sizeof(query), "update member set m_psw ='%s'  where m_id = '%s'", entered_psw1, entered_id);
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);

									printf("		비밀번호 변경 완료!\n");
								}
							}
							// 로그아웃
							else if (select2 == 6)
							{
								sprintf_s(query, sizeof(query), "drop view personalcart");
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								break;
							}
							else
							{
								printf("	잘못 입력하셨습니다.\n");
								goto back7;
							}
						}
					}
					else
					{
						printf("	회원 ID와 비밀번호가 일치하지 않습니다. 다시 입력해주세요.\n");
						goto back2;
					}
				}

				// 회원 가입
				else if (select1 == 2)
				{
					printf("\n");
					printf("가입할 회원의 정보를 입력해주세요.\n");
					
					// ID 중복 체크 
					back3:
					printf("	ID: ");
					scanf_s("%s", entered_id, sizeof(entered_id));


					sprintf_s(query, sizeof(query), "select * from member where m_id = '%s'", entered_id);
					query_stat = mysql_query(connection, query);
					sql_result = mysql_store_result(connection);

					if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("	이미 존재하는 ID입니다.\n");
						goto back3;
					}

					// 비밀번호가 같은지 확인
					back4:
					printf("	password: ");
					scanf_s("%s", entered_psw1, sizeof(entered_psw1));
					printf("	password 확인: ");
					scanf_s("%s", entered_psw2, sizeof(entered_psw2));

					if (strcmp(entered_psw1, entered_psw2))
					{
						printf("	비밀번호가 일치하지 않습니다.\n");
						goto back4;
					}
					else
					{
						printf("	이름: ");
						scanf_s("%s", entered_name, sizeof(entered_name));
						printf("	전화번호: ");
						scanf_s("%s", entered_phone, sizeof(entered_phone));

						sprintf_s(query, sizeof(query), "insert into member values ('%s', '%s', '%s', '%s')", entered_id, entered_psw1, entered_name, entered_phone);
						query_stat = mysql_query(connection, query);
						sql_result = mysql_store_result(connection);

						printf("	회원가입 완료!\n");
						printf("\n");
					}
				}

				// 회원 탈퇴
				else if (select1 == 3)
				{
					printf("\n");
					printf("탈퇴할 회원의 정보를 입력해주세요.\n");
					printf("	ID: ");
					scanf_s("%s", entered_id, sizeof(entered_id));
					printf("	password: ");
					scanf_s("%s", entered_psw1, sizeof(entered_psw1));

					sprintf_s(query, sizeof(query), "delete from member where m_id = '%s' and m_psw = '%s'", entered_id, entered_psw1);
					query_stat = mysql_query(connection, query);
					sql_result = mysql_store_result(connection);

					printf("	회원탈퇴 완료!\n");
					printf("\n");
				}

				// 회원 현황
				else if (select1 == 4)
				{
					printf("\n");
					printf("===================회원 현황==================\n");

					query_stat = mysql_query(connection, "select m_id, m_name, m_phone from member");
					sql_result = mysql_store_result(connection);

					printf("	회원 ID		이름	   전화번호\n");
					printf("==============================================\n");
					while (sql_row = mysql_fetch_row(sql_result))
					{
						for (int i = 0; i < mysql_num_fields(sql_result); i++)
							printf("%15s", sql_row[i]);
						printf("\n");
					}
					printf("==============================================\n");
				}

				else
				{
					printf("\n");
					printf("잘못 입력하셨습니다.\n");
					goto back1;
				}

				printf("\n");
			}
		}

		mysql_free_result(sql_result);
	}

	mysql_close(connection);
}
