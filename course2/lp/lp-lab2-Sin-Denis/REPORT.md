#№ Отчет по лабораторной работе №2
## по курсу "Логическое программирование"

## Решение логических задач

### студент: Cин Д.Д. М80-207Б-18

## Результат проверки

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |
| Левинская М.А.|    14/11     |     4         |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*: решение д.б. единственным, математик и биолог не знают общего языка (иначе им не нужен был переводчик).


## Введение

Для решения задач можно использовать метод раассуждений, таблицы истинности, а также другие различные методы. Но очевидно, что если задача будет сложной и в ней будет много условий и большое количество входных данных, то решить задачу обычными методами будет сложно. Для решения логических задач удобным языком является пролог, так как с помощью предикатов мы можем задать факты из условия задач очень быстро и без лишних проблем, в то время как в других императивных языках нам бы пришлось писать различные структуры или классы, затем определять отношения между объектами, что очень долго и сложнее. И в итоге решение логических задач сводится к перебору всех вариантов и отсечения тех, которые не соответствуют условию задачи. Ну и все что нам необходимо для решения логических задач присутствует в прологе.

## Задание
Вариант 7

На одном международном конгрессе встретились 4 делегата из разных стран. Каждый из них владел двумя языками из 4 (английский, французский, немецкий, итальянский). Однако, оказалось, что не было такого языка, на котором они могли бы говорить вчетвером. И был только один язык, на котором могли вести беседу трое из них. Никто из делегатов не владеет французским и немецким языками одновременно. Хотя физик не говорит по-английски, он может служить переводчиком, если математик и биолог захотят поговорить друг с другом. Биолог говорит по-немецки и может говорить с химиком, хотя тот не знает ни одного немецкого слова. Физик, математик и химик не могут беседовать втроем на одном языке. Какими двумя языками владеет каждый из них?

## Принцип решения

Для решения задачи мы делаем перебор всех возможных вариантов владения делегатами двумя языками из четырех возможных. Затем из всех данных вариантов мы отсекаем те из них, которые не соответсвуют условию задачи.

Пример реализации перебора:

```
solve(Solve) :-
	langs(Langs),
	Solve = [Fizik, Biolog, Matematik, Himik],
	lang(ALang1), lang(ALang2), before(ALang1, ALang2, Langs),
	lang(BLang1), lang(BLang2), before(BLang1, BLang2, Langs),
	lang(CLang1), lang(CLang2), before(CLang1, CLang2, Langs),
	lang(DLang1), lang(DLang2), before(DLang1, DLang2, Langs),
	Fizik = delegat(ALang1, ALang2),
	Biolog = delegat(BLang1, BLang2),
	Matematik = delegat(CLang1, CLang2),
	Himik = delegat(DLang1, DLang2),
```

При переборе мы сразу отбрасываем вариант, когда один делегат может владеть одним и тем же языком, а также вариант повторение, когда два языка просто меняются местами. Это все делается с помощью предиката `before`.

Примеры наложения условий задачи на перебор:
```
speak_common_lang([D1, D2, D3, D4]) :-
	speak(D1, Lang),
	speak(D2, Lang),
	speak(D3, Lang),
	speak(D4, Lang).

three_speak_lang([D1, D2, D3, D4]) :-
	speak(D1, Lang), speak(D2, Lang), speak(D3, Lang);
	speak(D1, Lang), speak(D2, Lang), speak(D4, Lang);
	speak(D1, Lang), speak(D3, Lang), speak(D4, Lang);
	speak(D2, Lang), speak(D3, Lang), speak(D4, Lang).

not_speak_french_german([D1, D2, D3, D4]) :-
	(not(speak(D1, french)); not(speak(D1, german))),
	(not(speak(D2, french)); not(speak(D2, german))),
	(not(speak(D3, french)); not(speak(D3, german))),
	(not(speak(D4, french)); not(speak(D4, german))).
```
Данные предикаты используются в главном предикате `solve`, где происходит перебор и отброс вариантов, которые не удовлетворяют условию.

Итоговый ответ задачи такой:
```
?- print_solve.
Fizik speak french and italian
Biolog speak german and italian
Matematik speak english and french
Himik speak english and italian

Fizik speak german and italian
Biolog speak english and german
Matematik speak english and german
Himik speak english and french

Fizik speak german and italian
Biolog speak english and german
Matematik speak english and german
Himik speak english and italian

Fizik speak german and italian
Biolog speak english and german
Matematik speak english and italian
Himik speak english and french

Fizik speak german and italian
Biolog speak english and german
Matematik speak german and italian
Himik speak english and french

Fizik speak german and italian
Biolog speak german and italian
Matematik speak english and german
Himik speak english and italian

Fizik speak german and italian
Biolog speak german and italian
Matematik speak english and german
Himik speak french and italian

true .
```
## Выводы
В данной лабораторной работе с помощью языка логического программирования prolog была решена логическая задача. Логическая задача была решена с помощью полного перебора, но другое более лучшее решение найти сложно. Prolog в данной задаче представился очень мощным инструментом, мы в целом не думаем о том как будет находится решение, а просто пишем условие задачи в виде фактов, а далее запускаем перебор. Как и в первой лабораторной работе при таком программировании нужно было мыслить иначе, но сейчас уже было гораздо легче. Самое интересное при программировании в таком стиле мышления это красота самого решения и небольшое количество строк.
