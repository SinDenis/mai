# Отчет по курсовому проекту
## по курсу "Логическое программирование"

### студент: Син Д.Д. М80-207Б-18

## Результат проверки

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |
| Левинская М.А.|     7/12     |      5        |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*

## Введение

В результате выполнения курсового проекта я получу базовые навыки написания программ на языке логического программирования, а также смогу познакомитсья в целом с парадигмой логического программирования и сравнить написание программ на логических языках программирования и императивных.

## Задание

 1. Создать родословное дерево своего рода на несколько поколений (3-4) назад в стандартном формате GEDCOM с использованием сервиса MyHeritage.com 
 2. Преобразовать файл в формате GEDCOM в набор утверждений на языке Prolog, используя следующее представление: ...
 3. Реализовать предикат проверки/поиска .... 
 4. Реализовать программу на языке Prolog, которая позволит определять степень родства двух произвольных индивидуумов в дереве
 5. [На оценки хорошо и отлично] Реализовать естественно-языковый интерфейс к системе, позволяющий задавать вопросы относительно степеней родства, и получать осмысленные ответы. 

## Получение родословного дерева

Для создания родословного дерева я использовал сервис https://www.myheritage.com/. В итоге в родословном дереве получилось 40 индивидов и 4 поколения.

## Конвертация родословного дерева

Для конвертации родословного дерева из файла SinTree.ged я использовал язык python, так как я его достаточно неплохо знаю и для него написаны библиотеки на все случаи жизни. 

В программе parser.py есть 2 класса Person и Family. При парсинге проверяются метки INDI, NAME, SURN, SEX, FAM, HUSB, WIFE, CHIL. После данных меток идет соответсвующая им информация, из которой конструируются объекты классов Person и Family. Из объектов составляются списки. Таким образом мы получаем список всех индивидов и список семей.
Далее надо просто пройтись по этим спискам и представить дерево в нужном представлении.

`Функция parse()`
```
def parse(file_name):
	persons = {}
	families = {}
	person_id = None
	person_name = None
	person_surname = None
	person_sex = None
	fam_idx = None
	with  open(file_name, 'r') as f:
		for line in f:
			if 'INDI' in line:
				person_id = line.split()[1]
			elif 'NAME' in line:
				arr = line.split()
				if len(arr) == 5:
					person_name = arr[2] + ' ' + arr[3]
				else:
					person_name = arr[2]
			elif 'SURN' in line:
				person_surname = line.split()[2]
			elif 'SEX' in line:
				person_sex = line.split()[2]
				persons[person_id] = (Person(
					person_id,
					person_name,
					person_surname,
					person_sex
					))
			elif 'FAM\n' in line:
				families[line.split()[1]] = Family(line.split()[1])
				fam_idx = line.split()[1]
			elif 'HUSB' in line:
				families[fam_idx].add_husband(line.split()[2])
			elif 'WIFE' in line:
				families[fam_idx].add_wife(line.split()[2])
			elif 'CHIL' in line:
				families[fam_idx].add_child(line.split()[2])
	return persons, families
```

Данная функция возвращает список индивидов и семей.

`Функция get_fact_sex(persons)`
```
def get_fact_sex(persons):
	for person in persons:
		print('sex(\'{}\', {}).'.format(
			persons[person].get_full_name(),
			persons[person].sex.lower()))
```

`Функция get_fact_parent(families, persons)`
```
def get_fact_parent(families, persons):
	for key in families:
		for child in families[key].childs:
			husb_id = families[key].husb_id
			wife_id = families[key].wife_id
			if (husb_id):
				print('parent(\'{}\', \'{}\').'.format(
					persons[husb_id].get_full_name(),
					persons[child].get_full_name()))
			if (wife_id):
				print('parent(\'{}\', \'{}\'.)'.format(
					persons[wife_id].get_full_name(),
					persons[child].get_full_name()))
```

Данные функции преобразуют данные в дерево с необходимым по варианту представлением, которое представляется из семя набор фактов на языке prolog.

## Предикат поиска родственника

По варианту необходиму написать предикат поиска деверя, что другими словами брата мужа.

`Предикат brother_in_law(X,Y)`
```
brother(X, Y) :-
	parent(Q, X),
	parent(Q, Y),
	sex(Q, m),
	sex(X, m),
	not(X = Y).

husband(X, Y) :-
	parent(X, Q),
	parent(Y, Q),
	not(X = Y),
	sex(X, m), !.

brother_in_law(X, Y) :-
	husband(Q, Y),
	brother(X, Q).
```
Предикат работает следующим образом. Он ищет мужа  Y, а затем его брата.

`Пример работы предиката:`
```
?- brother_in_law(X, 'Kim Elena').
X = 'Sin Aleksandr' .

?- brother_in_law(X, 'I Ok Suni').
X = 'Lim En Se' ;
false.
```

## Определение степени родства

Для определении степени родства между двумя индивидами в дереве необходимо использовать поиск в дереве между двумя узлами дерева. Я решил использовать поиск с итерационным заглублением, где лимит для него задается от 1 до 10.

`Предикат relative(Rel, X, Y)`

```
move(Vert1, Vert2) :-
	brother(Vert2, Vert1).
move(Vert1, Vert2) :-
	sister(Vert2, Vert1).
move(Vert1, Vert2) :-
	parent(Vert2, Vert1).
move(Vert1, Vert2) :-
	parent(Vert1, Vert2).

prolong([X|T], [Y, X|T]) :-
	move(X, Y), not(member(Y, [X|T])).

depth_id([Finish|T], Finish, [Finish|T], 0).
depth_id(Path, Finish, R, N) :-
    N > 0,
    prolong(Path, NewPath),
    N1 is N - 1,
    depth_id(NewPath, Finish, R, N1).

search_id(Start, Finish, Path, DepthLimit) :-
    depth_id([Start], Finish, Res, DepthLimit),
    reverse(Res, Path).

search_id(Start, Finish, Path) :-
	generate(1, Level),
	search_id(Start, Finish, Path, Level).

generate(X,X).
generate(Y,X) :- Y<10, 
		Y1 is Y+1, 			generate(Y1,X).

short_relative('mother', X, Y) :- mother(X, Y).
short_relative('father', X, Y) :- father(X, Y).
short_relative('son', X, Y) :- son(X, Y).
short_relative('daughter', X, Y) :- daughter(X, Y).
short_relative('brother', X, Y) :- brother(X, Y).
short_relative('sister', X, Y) :- sister(X, Y).

path_transform([A, B|T], [R|X]) :-
	short_relative(R, A, B),
	path_transform([B|T], X).
path_transform([_], []).

relative(Rel, Human1, Human2) :-
	search_id(Human1, Human2, Path),
	path_transform(Path, Rel).

```
Данный предикат работает следующим образом. Сначала производится поиск в дереве от одного индивида до другого и мы получаем путь, который по сути является списком индивидов. Далее необходимо этот путь трансформировать в список отношений между каждой парой элементов в списке. Таким образом мы получим список, который показывает отношение родства.

`Пример работы:`
```
?- relative(Rel, 'Sin Denis', 'Sin Boris').
Rel = [brother] ;
Rel = [son, father] ;
Rel = [son, mother] .

?- relative(Rel, 'Sin Denis', 'Lim Ten Chury').
Rel = [son, daughter] ;
Rel = [brother, son, daughter] ;
Rel = [son, sister, son] ;
Rel = [son, sister, daughter] ;
Rel = [brother, son, sister, son] ;
Rel = [brother, son, sister, daughter] ;
Rel = [son, father, son, daughter] ;
Rel = [son, sister, brother, daughter] ;
Rel = [son, sister, sister, son] ;
Rel = [son, daughter, mother, daughter] ;
Rel = [son, daughter, mother, son] ;
Rel = [brother, son, sister, brother, daughter] ;
Rel = [brother, son, sister, sister, son] ;
Rel = [brother, son, daughter, mother, daughter] ;
Rel = [brother, son, daughter, mother, son] ;
Rel = [son, father, son, sister, son] ;
Rel = [son, father, son, sister, daughter] ;
Rel = [son, sister, son, mother, daughter] ;
Rel = [son, sister, daughter, mother, son] ;
Rel = [son, daughter, mother, sister, son] ;
Rel = [son, daughter, mother, brother, daughter] ;
Rel = [brother, son, sister, son, mother, daughter] ;
Rel = [brother, son, sister, daughter, mother, son] ;
Rel = [brother, son, daughter, mother, sister, son] ;
Rel = [brother, son, daughter, mother, brother, daughter] ;
Rel = [son, father, son, sister, brother, daughter] ;
Rel = [son, father, son, sister, sister, son] ;
Rel = [son, father, son, daughter, mother, daughter] ;
Rel = [son, father, son, daughter, mother, son] ;
Rel = [son, father, son, sister, son, mother, daughter] ;
Rel = [son, father, son, sister, daughter, mother, son] ;
Rel = [son, father, son, daughter, mother, sister, son] ;
Rel = [son, father, son, daughter, mother, brother, daughter] ;
false.
```

## Естественно-языковый интерфейс

Для написания естественно-языкового интерфейса необходимо написать грамматику вопросов английского языка, изменять базу знаний прлога при оопределенных запросах, а также строить дерево разбора английского вопроса и выдавать верный ответ.

`Грамматика английского вопроса`
```
question(question(R)) --> general_q(R).
question(question(R)) --> special_q(R).
question(question(R)) --> subject_q(R).

general_q(general_q(A, B, C, D)) --> help_verb(A), subject(B), predicate(C), addition(D).
special_q(special_q(A, B, C, D, E, F)) --> special_question(A), addition(B), help_verb(C), subject(D), predicate(E), addition(F).
subject_q(subject_q(A, B, C)) --> special_question(A), help_verb(B), addition(C).

subject(subject(A)) --> noun(A).
subject(subject(A)) --> pronoun(A).

addition(addition(A)) --> noun(A).
addition(addition(A)) --> pronoun(A).
addition(addition(A, B, C)) --> noun(A), preposition(B), noun(C).
addition(addition(A, B, C)) --> noun(A), preposition(B), pronoun(C).
addition(addition(A, B, C)) --> preposition(A), noun(B), ['and'], noun(C).
addition(addition([])) --> [].

predicate(predicate(R)) --> verb(R).
predicate(predicate([])) --> [].

help_verb(help_verb(S)) 				--> {member(S, ['Is', 'is', 'Does', 'does'])}, [S].
special_question(special_question(S))	--> {member(S, ['Who', 'How many', 'What'])}, [S].
preposition(preposition(S)) 			--> {member(S, ['of', 'for', 'between'])}, [S].
noun(noun(S)) 							--> {member(S, ['father', 'mother', 'sister', 'sisters', 'brother', 'brothers', 'son', 'sons', 'daughter', 'daughters', 'relationship'])}, [S].
noun(noun(S)) 							--> {sex(S, _)}, [S].
verb(verb(S)) 							--> {member(S, ['have', 'is'])}, [S].
pronoun(pronoun(S))						--> {member(S, ['him', 'her', 'he', 'she'])}, [S].
```

`Обработка запроса:`
```
get_rel(Lst) :-
	build_ast(
		Lst, question(general_q(
				help_verb('Is'), subject(noun(X)), predicate(_), addition(noun(Rel), preposition(_), noun(Y))))
		),
		asserta(person(Y)),
		relative([Rel], X, Y).

get_rel(Lst) :-
	build_ast(
		Lst, question(special_q(
			special_question('How many'), addition(noun(Rel)), help_verb(_), subject(noun(X)), predicate(verb(_)), addition(_)))
		),
		asserta(person(X)),
		concat(NewRel, Tmp, Rel), string_length(Tmp, 1),
		findall(Hum, relative([NewRel], Hum, X), L),
		length(L, Ans),
		write(X), write(' has '), write(Ans), write(' '), write(Rel), nl. 

get_rel(Lst) :-
	build_ast(
		Lst, question(subject_q(
			special_question('Who'), help_verb(_), addition(noun(Rel), preposition(_), pronoun('him'))))
		),
		person(Y),
		sex(Y, m),
		relative([Rel], Ans, Y),
		write(Rel), write(' for '), write('him'), write(' is '), write(Ans), nl.
```

`Примеры работы:`
```
?- get_rel(['How many', 'sons', 'does', 'Sin Dmitry', 'have']).
Sin Dmitry has 2 sons
true .

?- get_rel(['Who', 'is', 'son', 'of', 'him']).
son for him is Sin Denis
true ;
son for him is Sin Boris
true .

?- get_rel(['Is', 'Sin Denis', 'brother', 'for', 'Sin Boris']).
true .

?- get_rel(['Is', 'Sin Denis', 'sinster', 'for', 'Sin Boris']).
false.

?- get_rel(['Is', 'Sin Denis', 'son', 'for', 'Lim Olga']).
true .

?- get_rel(['What', 'is', 'relationship', 'between', 'Sin Denis', 'and', 'Lim Ten Chury']).
Relationship between Sin Denis and Lim Ten Chury is [son,daughter]
true ;
Relationship between Sin Denis and Lim Ten Chury is [brother,son,daughter]
true ;
Relationship between Sin Denis and Lim Ten Chury is [son,sister,son]
true ;
Relationship between Sin Denis and Lim Ten Chury is [son,sister,daughter]
true ;
Relationship between Sin Denis and Lim Ten Chury is [brother,son,sister,son]
true .

?- get_rel(['How many', 'brothers', 'does', 'he', 'have']).
He has 1 brothers
true .

?- get_rel(['Who', 'is', 'brother', 'of', 'him']).
brother for him is Lim En Se
true .
```

## Выводы
Курсовой проект был непростым. Пришлось привыкать писать на языке prolog. И после того как ты пишешь на обычных языках программировать типа C/C++ или python, довольно непросто писать на языке логического программирования. В ходе работы над курсовым проектом пришлось задуматься над тем как обычные алогоритмы написать на prolog, например в задании на определении степени родства необходимо было написать поиск в графе. В целом, когда пишешь на прологе понимаешь, что к задаче нужно подходить несколько иначе чем обычно, и это сбивает с толку. Самым интересным заданием в курсовом проекте для меня стало написание естественно-языкового интерфейса (хотя он и получился немного кривым), так как тут затрагиваются вещи, которые действительно применимы в прикладной разработке, например написание грамматик, построение структуры и анализ текста. И на самом деле, prolog в этой сфере блистает во всей красе. В итоге для себя я пришел к выводу, что prolog действительно очень интересный язык, он заставляет менять мышление, конечно, он в каких-то обычных вещах очень непроизводительный, но вот в науке и для анализа текстов он подходит отлично.
