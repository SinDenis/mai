% Студент: Син Денис
% Группа: М80-207Б-18
% Номер по списку: 23

% Списки
% Variant: 5
% Удаление N первых элементов.

% Числовые списки
% Вариант: 8
% Вычисление среднего арифметического элементов.

% Стандартные предикаты на списках
my_member(X, [X|Y]).
my_member(X, [Y|Z]) :-
	my_member(X, Z).

my_length([], 0).
my_length([X|Y], T) :-
	my_length(Y, Q),
	T is Q + 1.

my_append([], L, L).
my_append([X | R1], L2, [X | R3]) :-
	my_append(R1, L2, R3).

my_remove(X, [X|R], R).
my_remove(X, [Y|R], [Y|T]) :-
	my_remove(X, R, T).

my_sublist(S, L) :-
	my_append(L1, L2, L),
	my_append(S, L3, L2).

my_permute([], []).
my_permute(L1, [X | R2]) :-
	my_remove(X, L1, L2),
	my_permute(L2, R2).

% Удаление N первых элементов без стандартных предикатов.
delete(X, 0, X).
delete([X|Y], N, Z) :-
	delete(Y, Q, Z),
	N is Q + 1.

% Удаление N первых элементов с помощью стандартных предикатов
delete2(X, N, Y) :-
	my_append(A, Y, X),
	my_length(A, N).

% Вычисление среднего арифметического элементов.
sum([], 0).
sum([X|Y], S) :-
	sum(Y, Q),
	S is Q + X.

arithm_mean(X, T) :-
	length(X, L),
	sum(X, P),
	T is P / L.

% Пример использования двух предикатов из пункта 3 и 4
% Придикат подсчета среднего арифметического списка без
% первых N элементов
arithm_mean_without_first_n(L, N, T) :-
	delete(L, N, P),
	arithm_mean(P, T).
