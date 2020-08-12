# Nullable типы
Nullable типы используются, чтобы дать возможность переменной иметь значение null.
Любой тип можно указать как nullable:
```
var number: Int? = 42
number = null

var nonNullNumber: Int = 42
nonNullNumber = null // Ошибка: Невозможно присвоить переменной типа Int значение null
```

| [< Вступление (типы)](types.md) | [Алиасы типов >](aliases.md) |
| - | - |