## bs-BigInteger.js

[BuckleScript](https://github.com/bloomberg/bucklescript) bindings over [BigInteger.js](https://github.com/peterolson/BigInteger.js)@1.6.x

## How do I use this?

For specifics, see the [documentation for BigInteger@1.6.47, from which this library was based](https://www.npmjs.com/package/big-integer/v/1.6.47).

## Status
As far as I am aware, all methods from version 1.6.47 are supported, to some extent.  

Notable omissions are:
 * *the constructor* (does not support alphabet or caseSensitive arguments).
 * `isProbablePrime` (does not support iterations or rng arguments)
 * `isInstance` (we have a type checker!!!!)
 * `randBetween` (does not support rng argument)
 * `toString` (does not support alphabet argument)

There are even some minor improvements, such as variant types for comparison operations, rather than `-1`, `0`, `1`.

However, there is no in-library error handling! In particular, if you do something crazy with the `String(string) polymorphic argument, you may see runtime errors unless you catch and handle them:

```ReasonMl
let x = BigInteger.bigInt(`String("IAmNotANumber"));
Js.log("I am unreachable due to an unhandled type error above");
```

Add as much error handling to your own code as you think appropriate given your use case, for example:

### Option variant
```ReasonMl
let x =
  try (Some(BigInteger.bigInt(`String("IAmNotAValidNumber")))) {
  | _ => None
  };

switch (x) {
| Some(bigInteger) => Js.log2("I'm a bigInt", bigInteger)
| None => Js.log("Oh no, there was an error")
};
// Logs "Oh no, there was an error".
```

### Belt.Result (polymorphic variant error type)
```ReasonMl
let suspiciousFunction = stringOfInt =>
  try (Belt.Result.Ok(BigInteger.bigInt(`String(stringOfInt)))) {
  | Js.Exn.Error(e) =>
    Belt.Result.Error(
      switch (Js.Exn.name(e)) {
      | Some(error) =>
        switch (error) {
        | "Error" => `BigIntegerError(e)
        | _ => `UnmatchedError(e)
        }
      | None => `SomeUnknownError
      },
    )
  };

let handleResult =
  fun
  | Belt.Result.Ok(bigInteger) => Js.log(bigInteger)
  | Belt.Result.Error(err) =>
    switch (err) {
    | `BigIntegerError(_) => Js.log("1")
    | `UnmatchedError(_) => Js.log("2")
    | `SomeUnknownError => Js.log("3")
    };

suspiciousFunction("IamNotValid")->handleResult;
// ^^ Logs 1
suspiciousFunction("777")->handleResult;
// ^^ Logs "Integer { value: 777n }"
```

### Exception

For convenience, this library exports `exception BigIntegerException`.  You should prefer the other options above though! 

```ReasonMl
let suspiciousFunction = stringOfInt =>
  try (BigInteger.bigInt(`String(stringOfInt))) {
  | _ => raise(BigInteger.BigIntegerException)
  };

switch (suspiciousFunction("I am not a valid number")) {
| bigInteger => Js.log(bigInteger)
| exception BigInteger.BigIntegerException => Js.log("There was an error!")
};
// ^^ Logs "error"

switch (suspiciousFunction("777")) {
| bigInteger => Js.log(bigInteger)
| exception BigInteger.BigIntegerException => Js.log("There was an error!")
};
// ^^ Logs "Integer { value: 777n }"

```

# Contributing
Contributions are welcome, especially for the small portions of this library that lacks coverage by typings.