#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <mysql.h>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1007"
#define DB_NAME "myDB" // DB ����

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
	
	mysql_init(&conn); // DB �ʱ�ȭ
	
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	// DB ����
	
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
				printf("1. �α��� \n");
				printf("2. ȸ�� ���� \n");
				printf("3. ȸ�� Ż�� \n");
				printf("4. ȸ�� ��Ȳ \n");
				printf("0. ���� \n");
				printf("������ ��ȣ�� �Է����ּ���: ");
				scanf_s("%d", &select1);

				if (select1 == 0)
					break;

				// �α���
				else if (select1 == 1)
				{
					printf("\n");
					printf("	�α����� ȸ���� ������ �Է����ּ���.\n");
					
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
						printf("	==============================�α��� ����!==============================\n");
						printf("\n");

						sprintf_s(query, sizeof(query), "create view personalcart as select p_category, p_name, p_price, c_p_num from cart, product where c_p_id = product.p_id and c_id = '%s'", entered_id);
						query_stat = mysql_query(connection, query);
						sql_result = mysql_store_result(connection);

						back6:
						printf("	===============================��ǰ Ȯ��================================\n");
						printf("	��ǰ��ȣ	�з�		   �̸�		    ���	    ����\n");
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
							printf("	1. ��ٱ��� ��� \n");
							printf("	2. ��ٱ��� Ȯ�� \n");
							printf("	3. �ֹ��ϱ� \n");
							printf("	4. �ֹ� Ȯ�� \n");
							printf("	5. ��й�ȣ ���� \n");
							printf("	6. �α� �ƿ� \n");
							printf("	������ ��ȣ�� �Է����ּ���: ");
							scanf_s("%d", &select2);
							printf("\n");

							// ��ٱ��� ���
							if (select2 == 1)
							{
								printf("		��ٱ��Ͽ� ���� ��ǰ ��ȣ: ");
								scanf_s("%d", &product_id, sizeof(product_id));
								printf("		����: ");
								scanf_s("%d", &product_num, sizeof(product_num));

								sprintf_s(query, sizeof(query), "insert into cart values('%s','%d','%d')", entered_id, product_id, product_num);
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);

								printf("		��ٱ��Ͽ� �߰��Ǿ����ϴ�.\n");
							}
							// ��ٱ��� Ȯ��
							else if (select2 == 2)
							{
								sprintf_s(query, sizeof(query), "select * from personalcart");
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);

								printf("		=====================��ٱ��� Ȯ��=====================\n");
								printf("		��ǰ�з�	�̸�		����		����\n");
								printf("		=======================================================\n");

								while (sql_row = mysql_fetch_row(sql_result))
								{
									for (int i = 0; i < mysql_num_fields(sql_result); i++)
										printf("		%5s", sql_row[i]);
									printf("\n");
								}

								printf("		��ٱ��Ͽ� ��� ��ü ��ǰ�� �ֹ��Ͻðڽ��ϱ�? (Y/N) : ");
								scanf_s("%s", answer, sizeof(answer));

								if (strcmp(answer, "Y"))
								{
									goto back7;
								}
								else
								{
									// �ֹ���ȣ ����
									sprintf_s(query, sizeof(query), "select * from orderlist");
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);
									
									while (sql_row = mysql_fetch_row(sql_result))
									{
										order_id++;
									}
									order_id += 10;

									// ��� ��Ȳ �ľ� �� �ֹ� �������� Ȯ��
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
							// �ֹ��ϱ�
							else if (select2 == 3)
							{
								printf("		�ֹ��� ��ǰ ��ȣ: ");
								scanf_s("%d", &product_id, sizeof(product_id));
								printf("		����: ");
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
									printf("		��� �������� �ֹ��� �����߽��ϴ�.\n");
							}
							// �ֹ� Ȯ��
							else if (select2 == 4)
							{
								sprintf_s(query, sizeof(query), "select distinct o_id, m_name, p_name, p_price, o_p_num from orderlist, member, product, cart where orderlist.o_m_id = member.m_id and orderlist.o_p_id = product.p_id and o_m_id = '%s'", entered_id);
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);

								printf("		====================================�ֹ� Ȯ��===================================\n");
								printf("		�ֹ���ȣ	�ֹ���		��ǰ�̸�	����		����\n");
								printf("		================================================================================\n");

								while (sql_row = mysql_fetch_row(sql_result))
								{
									for (int i = 0; i < mysql_num_fields(sql_result); i++)
										printf("		%5s", sql_row[i]);
									printf("\n");
								}
							}
							// ��й�ȣ ����
							else if (select2 == 5)
							{
								back5:
								printf("		������ password: ");
								scanf_s("%s", entered_psw1, sizeof(entered_psw1));
								printf("		������ password Ȯ��: ");
								scanf_s("%s", entered_psw2, sizeof(entered_psw2));

								if (strcmp(entered_psw1, entered_psw2))
								{
									printf("		��й�ȣ�� ��ġ���� �ʽ��ϴ�.\n");
									goto back5;
								}
								else
								{
									sprintf_s(query, sizeof(query), "update member set m_psw ='%s'  where m_id = '%s'", entered_psw1, entered_id);
									query_stat = mysql_query(connection, query);
									sql_result = mysql_store_result(connection);

									printf("		��й�ȣ ���� �Ϸ�!\n");
								}
							}
							// �α׾ƿ�
							else if (select2 == 6)
							{
								sprintf_s(query, sizeof(query), "drop view personalcart");
								query_stat = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								break;
							}
							else
							{
								printf("	�߸� �Է��ϼ̽��ϴ�.\n");
								goto back7;
							}
						}
					}
					else
					{
						printf("	ȸ�� ID�� ��й�ȣ�� ��ġ���� �ʽ��ϴ�. �ٽ� �Է����ּ���.\n");
						goto back2;
					}
				}

				// ȸ�� ����
				else if (select1 == 2)
				{
					printf("\n");
					printf("������ ȸ���� ������ �Է����ּ���.\n");
					
					// ID �ߺ� üũ 
					back3:
					printf("	ID: ");
					scanf_s("%s", entered_id, sizeof(entered_id));


					sprintf_s(query, sizeof(query), "select * from member where m_id = '%s'", entered_id);
					query_stat = mysql_query(connection, query);
					sql_result = mysql_store_result(connection);

					if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("	�̹� �����ϴ� ID�Դϴ�.\n");
						goto back3;
					}

					// ��й�ȣ�� ������ Ȯ��
					back4:
					printf("	password: ");
					scanf_s("%s", entered_psw1, sizeof(entered_psw1));
					printf("	password Ȯ��: ");
					scanf_s("%s", entered_psw2, sizeof(entered_psw2));

					if (strcmp(entered_psw1, entered_psw2))
					{
						printf("	��й�ȣ�� ��ġ���� �ʽ��ϴ�.\n");
						goto back4;
					}
					else
					{
						printf("	�̸�: ");
						scanf_s("%s", entered_name, sizeof(entered_name));
						printf("	��ȭ��ȣ: ");
						scanf_s("%s", entered_phone, sizeof(entered_phone));

						sprintf_s(query, sizeof(query), "insert into member values ('%s', '%s', '%s', '%s')", entered_id, entered_psw1, entered_name, entered_phone);
						query_stat = mysql_query(connection, query);
						sql_result = mysql_store_result(connection);

						printf("	ȸ������ �Ϸ�!\n");
						printf("\n");
					}
				}

				// ȸ�� Ż��
				else if (select1 == 3)
				{
					printf("\n");
					printf("Ż���� ȸ���� ������ �Է����ּ���.\n");
					printf("	ID: ");
					scanf_s("%s", entered_id, sizeof(entered_id));
					printf("	password: ");
					scanf_s("%s", entered_psw1, sizeof(entered_psw1));

					sprintf_s(query, sizeof(query), "delete from member where m_id = '%s' and m_psw = '%s'", entered_id, entered_psw1);
					query_stat = mysql_query(connection, query);
					sql_result = mysql_store_result(connection);

					printf("	ȸ��Ż�� �Ϸ�!\n");
					printf("\n");
				}

				// ȸ�� ��Ȳ
				else if (select1 == 4)
				{
					printf("\n");
					printf("===================ȸ�� ��Ȳ==================\n");

					query_stat = mysql_query(connection, "select m_id, m_name, m_phone from member");
					sql_result = mysql_store_result(connection);

					printf("	ȸ�� ID		�̸�	   ��ȭ��ȣ\n");
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
					printf("�߸� �Է��ϼ̽��ϴ�.\n");
					goto back1;
				}

				printf("\n");
			}
		}

		mysql_free_result(sql_result);
	}

	mysql_close(connection);
}
