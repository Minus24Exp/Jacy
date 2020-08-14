# Типы
В Yocto типы это ничто иное как объект описывающий список полей.

## Объединение типов
Два или несколько типов можно объединить в один алиас, и тогда этот алиас сможет принимать любой из этих типов.

Пример:
```
type IntOrFloat = Int | Float

var my_var: IntOrFloat
// "my_var" может быть Int или Float
my_var = 10
my_var = 123.123
```

## Слияние типов
Два или несколько типов можно совместить в один. В этом случае в новом типе будут присутствовать поля каждого из объектов. 
Если два типа имеют поля с одинаковыми именами, то в новом типе останется поле последнего из этих двух типов в слиянии.

Пример:
```
type AB = A & B

class FirstClass {
    var my_var: Int
}
class SecondClass {
    var my_var: String
}

type FirstSecond = FirstClass & SecondClass

// FirstSecond содержит поле "my_var" типа String
```

| [< Начало работы](../getting_started.md) | [Nullable типы >](nullable.md) |
| - | - |
