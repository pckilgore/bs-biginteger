//*
// Why all the comments?  Well, typing libraries was confusing as hell when I
// started learning ReasonML and Bucklescript and so I wanted an example to
// show new developers that explains exactly what's happening for beginners
// coming from a Javascript background.
//
// ***I am assuming you have a fairly basic knowledge of Reason syntax and
// concepts like pattern matching, variants, and options.***
//
// That said, because this is for beginners, I'll probably sometimes say things
// that aren't 100% accurate, but that I know might clarify a concept more
// immediately than the real, detailed answer.
//
// To the learners, I apologize for this shortcut and encourage you to question
// my analogies.  To the experienced, I'm just sorry, but if you feel I'm
// seriously misleading someone please let me know!
//
// If you have any questions file an issue!  Or find me on the ReasonML discord
// @pckilgore#5005
///

//*
type t;
// What is `type t`?
//
// It is a ocaml convention for "the type of this module".  So if the module was
// named "Fish", `type t` would be the fish.  We could just as easily call it
// anything else, even `type fish`, but then we'd be referring to it as
// `Fish.fish` which seems silly, right? So we call it `type t` and refer to it
// as `Fish.t` and by convention know `t` means the module's type.
//
// A ReasonML module is a type packaged with its behavior.  In this way, it is
// similar to an Object-Oriented language's concept of `class`.
//
// So here, `type t` is the BigInteger data structure, packaged with the methods
// we can use to operate on that type.
//
// Because we don't really know (or, honestly, care) about _how_ the BigInteger
// library implements the BigInteger type, we just declare it here, which means
// it is an "abstract type", which I always think of as, "a type that must be
// used consistently by the functions that operate on it, but for which the
// particular implementation of the type and those functions are assumed to be
// correct".
//
// See below around the definition of `external abs` for an example of a
// non-abstract type t.
//
///

//*
exception BigIntegerException;
// What is this?  In Reason, exceptions are a special type of variant.  This
// library doesn't actually handle them for it's consumers, but to be nice,
// I declare this type here so that users of this library, should they want to
// handler errors themselves, can have a consistent way of communicating that
// fact to consumers of their code.
//
// Because a module might have multiple exceptions, we try to pick descriptive
// names for them.  This name isn't very descriptive! But really, that's OK
// because there are better ways to handle unexpected actions in a strongly
// typed language like ReasonML: variants!
//
// See the README for examples of other ways to handle errors in your code.
// Those examples apply to all sorts of libraries!
///

//*
// The function below constructs a new BigInteger (of type t) with Base 10.
//
// Type signature is outside the scope of this explanation, but to break it
// down just this one time:
//
// | [@bs.module] ... = "big-integer" |
// ------------------------------------
//   Tells Bucklescript there is a module you need to import called
//   "big-integer"
//
// | bigInt |
// ----------
//   Tells bucklescript that the "big-integer" module should be called
//   "bigInt".  In connection with the syntax above, this compiles to
//   `var bigInt = require('big-integer')`
//
// | bigInt: typeA => typeB |
// --------------------------
//   Tells bucklescript that bigInt is a function that takes an argument of
//   typeA and that it returns typeB.  Note that typeA and typeB aren't
//   actually the types below, just examples.  See below for the actual type.
//
// | ( [@bs.unwrap] ... ) |
// ------------------------
//   Tells bucklescript the type of the first (and only) argument to the bigInt
//   function is polymorphic (can be multiple types).  Tells Bucklescript to
//   expect a polymorphic variant next that describes all these types.
//
//  | [ | `String(string) | `Int(int) | `BigInt(t)] |
//  -------------------------------------------------
//    The polymorphic variant.  Note, I could have just as easily called these
//    [ | `Apple(string) | `Banana(int) | `Pear(t) ].  It is the type that is
//    in the constructor, e.g., `(string)`, that is important.  [@bs.unwrap]
//    tells Bucklescript to "unwrap" these variants during compilation to
//    javascript to the type in the constructor.  This lets the compiler
//    type-check your Reason code, but compile to clean, efficient javascript.
//
//  | => t |
//  --------
//    Tells bucklescript bigInt returns type t. See above about type t.
//    (I know this feels a little circular, and not very descriptive. But
//    remember type names are arbitrary.  We could have declared `type
//    bigInteger` above.  And then this would be `=> bigInteger` rather than
//    `=> t`.  But then in other Reason modules, we would see this type as
//    `BigInteger.bigInteger` (BigInteger is the module name, which comes from
//    the file name--in Reason all files are modules, but not all modules are
//    files).  Since that's rather verbose, we say `type t`, knowing that it
//    will be namespaced by the type system to `BigInteger.t` and that will be
//    clear to other programmers by convention.
//
///

/**
 * Initialize a new BigInteger
 */
[@bs.module]
external bigInt:
  ([@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "big-integer";

//*
// Construct a new BigInteger (type t!!) with Base N.
//
// It's hard to model javascript in Reason, particularly because arguments can
// be multiple types or completely optional.  There's lots of strategies to deal
// with this, described in the "Functions" section of the Bucklescript docs.
//
// Note that I chose not to support all four arguments documented for bigInt
// (base, alphabet, etc...). That was an arbitrary design decision. But we model
// higher-arity functions like this in a few different ways:
//
//  * Define X functions separately, where X is the number of arguments. I do
//    that here with two functions (bigInt and bigIntBaseN).
//
//  * Define one function with optional arguments and defaults.  I do that below
//    for the `fromArray` function.
///

/**
 * Initialize a new BigInteger of base N
 */
[@bs.module]
external bigIntBaseN:
  (
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]
  ) =>
  t =
  "big-integer";

//*
// [@bs.send] is how we tell bucklescript the external we are defining should
// be chained off the first argument to the function we are defining.  This is
// how you model object methods in Bucklescript.
//
// Because this is a functional language, we do not carry state around like an
// object might.  Rather, we choose what data we want to keep, type it, and then
// define operations on it (usually immutably).
//
// Remember the fish example above?  Maybe we want that fish to swim. Swim is a
// _behavior_ of the type Fish (`Fish.t`). In a _real_ ReasonML module it would
// look like:
//
// module Fish = {
//  type t = {position: int};
//  let swimForward = (t, howFar) => {position: t.position + howFar};
//  let swimBackward = (t, howFar) => {position: t.position - howFar};
// }
//
// So just like the fish example above, below, we first pass in the BigInteger
// we want to operate on (our `type t`), then the arguments for the operation.
//
// --ASIDE---
// Wait, why is `t` before `howFar`, I thought I read somewhere that....
//   ^^ https://www.javierchavarri.com/data-first-and-data-last-a-comparison/
// --END ASIDE--
//
// This is best explained via code example:
//
// So we can call, from reason:
// ```reason
// let x = BigInteger.bigInt(`Int(-10)); // assigns x to BigInteger.t of -10n
// let y = BigInteger.abs(x); // assigns y to BigInteger.t of 10n
// ```
// and it compiles to, roughly:
// ```javascript
// var x = bigInt(-10); // -10n
// var y = x.abs(); // 10n
// ```
///

/**
 * Returns the absolute value of a bigInt.
 */
[@bs.send]
external abs: t => t = "abs";

/**
 * Performs addition.
 */
[@bs.send]
external add:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "add";

//*
// Why `and_` and not `and`? Because `and` is a reserved word so we _have_ to
// change the name of this function, and by convention we use a `_` suffix.
///

/**
 * Performs the bitwise AND operation. The operands are treated as if they were
 * represented using two's complement representation
 */
[@bs.send]
external and_:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "and";

/**
 * Returns the number of digits required to represent a bigInt in binary.
 */
[@bs.send]
external bitLength: t => t = "bitLength";

//*
// Why the _ prefix ? Just signaling that `_compare` is "private".  We're going
// to be nice here and wrap this function's result in a variant.  If we typed it
// as a int, it would work, but the compiler would be confused (we only assign
// value to a -1, 0, 1.  And so it would ask consumers of this library to handle
// the case of any other possible number (even though we know that's impossible
// given the contract of BigInteger.js)).  Here we define the variant type:
type compareResult =
  | GreaterThan
  | LessThan
  | EqualTo;
// Then we define the internal function we will wrap...

/**
 * Performs a comparison between two numbers. If the numbers are equal, it
 * returns `0`. If the first number is greater, it returns `1`. If the first
 * number is lesser, it returns `-1`.
 */
[@bs.send]
external _compare:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => int =
  "compare";
// Then we wrap the function for our users (below).
//
// this doesn't _really_ hide the function (you can still call
// BigInteger._compare from another module if you want.).  Thats fine, and
// I want to give users that option.  But you can define an "interface" file
// if you truly want to hide implementation details from consumers. You
// might see these in other projects, with the `.rei` extension.
///

/**
 * Performs a comparison between two numbers. Returns a variant:
 *   * first number > second number => GreaterThan
 *   * first number < second number => LessThan
 *   * first number == second number => EqualTo
 *
 * Raises a BigIntegerException if the underlying BigInteger library does not
 * conform to its published API.
 */
let compare = (t, number) =>
  switch (_compare(t, number)) {
  | 0 => EqualTo
  | 1 => GreaterThan
  | (-1) => LessThan
  | _ => raise(BigIntegerException) // This should never happen!
  };

// We are going to repeat this pattern for compareAbs...

/**
 * Performs a comparison between the absolute value of two numbers. If the
 * numbers are equal, it returns `0`. If the first number is greater, it returns
 * `1`. If the first number is lesser, it returns `-1`.
 */
[@bs.send]
external _compareAbs:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => int =
  "compareAbs";

/**
 * Performs a comparison between the absolute value of two numbers.
 * Returns variant:
 *   * first number > second number => GreaterThan
 *   * first number < second number => LessThan
 *   * first number == second number => EqualTo
 *
 * Raises a BigIntegerException if the underlying BigInteger library does not
 * conform to its published API.
 */
let compareAbs = (t, number) =>
  switch (_compareAbs(t, number)) {
  | 0 => EqualTo
  | 1 => GreaterThan
  | (-1) => LessThan
  | _ => raise(BigIntegerException) // This should never happen!
  };

/**
 * Performs integer division, disregarding the remainder.
 */
[@bs.send]
external divide:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "divide";

//**
//  divmod is an odd op, because the library returns an object of the shape:
type divmodResult = {
  quotient: t,
  remainder: t,
};
// Recall `: t` above is just a `type t` or BigInteger.t.
//
// There are less verbose options here, but let's define bucklescript getters
// for the properties we want.
//  Warning! Things like this _CAN RESULT IN TYPE ERRORS_, so avoid it unless
//  you need to.
[@bs.get] external getQuotient: divmodResult => t = "quotient";
//                 ^^-func name ^^-type it works on ^^-name of js property
[@bs.get] external getRemainder: divmodResult => t = "remainder";
//
//  Internal function to retrieve the fields from the object/dict above.
//
[@bs.send]
external _divmod:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) =>
  divmodResult =
  "divmod";
//
// Below is the function we will expose that returns the actual record type.
////

/**
 * Performs division and returns a record with two properties: `quotient` and
 * `remainder`. The sign of the remainder will match the sign of the dividend.
 */
let divmod = (dividend, divisor) => {
  let divModObject = _divmod(dividend, divisor);
  let quotient = getQuotient(divModObject);
  let remainder = getRemainder(divModObject);
  {quotient, remainder};
};

/**
 * Checks if two numbers are equal.
 */
[@bs.send]
external equals:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "equals";

/**
 * Checks if the first number is greater than the second.
 */
[@bs.send]
external greaterOrEquals:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "greaterOrEquals";

/**
 * Checks if the first number is greater than or equal to the second.
 */
[@bs.send]
external greater:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "greater";

/**
 * Returns `true` if the first number is divisible by the second number,
 * `false` otherwise.
 */
[@bs.send]
external isDivisibleBy:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "isDivisibleBy";

/**
 * Returns `true` if the number is even, `false` otherwise.
 */
[@bs.send]
external isEven: t => bool = "isEven";

/**
 * Returns `true` if the number is negative, `false` otherwise. Returns `false`
 * for `0` and `-0`.
 */
[@bs.send]
external isNegative: t => bool = "isNegative";

/**
 * Returns `true` if the number is odd, `false` otherwise.
 */
[@bs.send]
external isOdd: t => bool = "isOdd";

/**
 * Return `true` if the number is positive, `false` otherwise.
 * Returns `false` for `0` and `-0`.
 */
[@bs.send]
external isPositive: t => bool = "isPositive";

/**
 * Returns `true` if the number is prime, `false` otherwise.
 */
[@bs.send]
external isPrime: t => bool = "isPrime";

/**
 * Returns `true` if the number is very likely to be prime, `false` otherwise.
 *
 * ***WARNING*** Does not support the entire BigInteger.js api!
 *
 */
[@bs.send]
external isProbablePrime: t => bool = "isProbablePrime";

/**
 * Returns `true` if the number is `1` or `-1`, `false` otherwise.
 */
[@bs.send]
external isUnit: t => bool = "isUnit";

/**
 * Return `true` if the number is `0` or `-0`, `false` otherwise.
 */
[@bs.send]
external isZero: t => bool = "isZero";

/**
 * Checks if the first number is lesser than the second.
 */
[@bs.send]
external lesser:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "lesser";

/**
 * Checks if the first number is less than or equal to the second.
 */
[@bs.send]
external lesserOrEquals:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "lesserOrEquals";

/**
 * Performs division and returns the remainder, disregarding the quotient. The
 * sign of the remainder will match the sign of the dividend.
 */
[@bs.send]
external mod_:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "mod";

/**
 * Finds the [multiplicative inverse](https://en.wikipedia.org/wiki/Modular_multiplicative_inverse)
 * of the number modulo `mod`.
 */
[@bs.send]
external modInv:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "modInv";

/**
 * Takes the number to the power `exp` modulo `mod`.
 */
[@bs.send]
external modPow:
  (
    t,
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]
  ) =>
  t =
  "modPow";

/**
 * Performs multiplication.
 */
[@bs.send]
external multiply:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "multiply";

/**
 * Adds one to the number.
 */ [@bs.send] external next: t => t = "next";

/**
 * Performs the bitwise NOT operation. The operands are treated as if they were
 * represented using two's complement representation.
 */
[@bs.send]
external not: t => t = "not";

/**
 * Checks if two numbers are not equal.
 */
[@bs.send]
external notEquals:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => bool =
  "notEquals";

/**
 * Performs the bitwise OR operation. The operands are treated as if they were
 * represented using two's complement representation.
 */
[@bs.send]
external or_:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "or";

/**
 * Performs exponentiation. If the exponent is less than `0`, `pow` returns `0`.
 * `bigInt.zero.pow(0)` returns `1`.
 */
[@bs.send]
external pow:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "pow";

/**
 * Subtracts one from the number.
 */
[@bs.send]
external prev: t => t = "prev";

/**
 * Shifts the number left by `n` places in its binary representation. If a
 * negative number is provided, it will shift right. Throws an error if `n` is
 * outside of the range `[-9007199254740992, 9007199254740992]`.
 */
[@bs.send]
external shiftLeft: (t, int) => t = "shiftLeft";

/**
 * Shifts the number right by `n` places in its binary representation. If a
 * negative number is provided, it will shift left. Throws an error if `n` is
 * outside of the range `[-9007199254740992, 9007199254740992]`.
 */
[@bs.send]
external shiftRight: (t, int) => t = "shiftRight";

/**
 * Squares the number
 */ [@bs.send] external square: t => t = "square";

/**
 * Performs subtraction.
 */
[@bs.send]
external subtract:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "subtract";

// See above for divmod
type toArrayResult = {
  value: array(int),
  isNegative: bool,
};
[@bs.get] external getValue: toArrayResult => array(int) = "value";
[@bs.get] external getIsNegative: toArrayResult => bool = "isNegative";

/**
 *  Internal BigInteger.toArray(radix)
 */
[@bs.send]
external _toArray:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) =>
  toArrayResult =
  "toArray";

/**
 * Converts a bigInt into an object with the properties "value" and
 * "isNegative." "Value" is `type array(int)` modulo the given radix.
 * "isNegative" is a boolean that represents the sign of the result.
 *
 *   - BigInteger.toArray(BigInteger.bigInt(`String("1e9")), `Int(10)) => {
 *       value: [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
 *       isNegative: false
 *     }
 *
 *   - Negative bases are supported.
 *   - Base 1 and base -1 are also supported.
 *   - Base 0 is only allowed for the number zero.
 */
let toArray = (number, radix) => {
  let toArrayObject = _toArray(number, radix);
  let value = getValue(toArrayObject);
  let isNegative = getIsNegative(toArrayObject);
  {value, isNegative};
};

/**
 *  Converts a bigInt into a native Javascript number. Loses precision for
 *  numbers outside the range `[-9007199254740992, 9007199254740992]`.
 */
[@bs.send]
external toJSNumber: t => int = "toJSNumber";

/**
 *  Performs the bitwise XOR operation. The operands are treated as if they were
 *  represented using two's complement representation.
 */
[@bs.send]
external xor:
  (t, [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]) => t =
  "xor";

/**
 * Alias to `compare`.
 */
let compareTo = compare;
/**
 * Alias to `equals`.
 */
let eq = equals;
/**
 * Alias to `greaterOrEquals`.
 */
let geq = greaterOrEquals;
/**
 * Alias to `greater`.
 */
let gt = greater;
/**
 * Alias for `lesserOrEquals`.
 */
let leq = lesserOrEquals;
/**
 * Alias for `lesser`.
 */
let lt = lesser;
/**
 * Alias for `subtract`.
 */
let minus = subtract;
/**
  * Alias for `notEquals`.
  */
let neq = notEquals;
/**
  * Alias for `divide`.
  */
let over = divide;
/**
  * Alias for `add`.
  */
let plus = add;
/**
  * Alias for `mod_`.
  */
let remainder = mod_;
/**
  * Alias for `times`.
  */
let times = multiply;

//=============static functions===============//

/**
 *  Internal BigInteger.fromArray
 */
[@bs.module "big-integer"]
external _fromArray: (array(int), int, bool) => t = "fromArray";

/**
 * Constructs a bigInt from an array of digits in base `base`. The optional
 * `isNegative` flag will make the number negative.
 */
let fromArray = (array, ~base=10, ~isNegative=false, ()) =>
  _fromArray(array, base, isNegative);

/**
 * Finds the greatest common denominator of `a` and `b`.
 */
[@bs.module "big-integer"]
external gcd:
  (
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]
  ) =>
  t =
  "gcd";

/**
 * Finds the least common multiple of `a` and `b`.
 */
[@bs.module "big-integer"]
external lcm:
  (
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]
  ) =>
  t =
  "lcm";

/**
 * Returns the largest of `a` and `b`.
 */
[@bs.module "big-integer"]
external max:
  (
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]
  ) =>
  t =
  "max";

/**
 * Returns the smallest of `a` and `b`.
 */
[@bs.module "big-integer"]
external min:
  (
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)]
  ) =>
  t =
  "min";

//*
// What is this "unit"?? Unit is a type that is a part of ReasonML, ocaml.  It
// means, depending on context "void" (a function can return unit) or
// "intentionally empty" (if used as the type of an argument.)
//
// The characters `()` and the word `unit` represent the same thing, but are
// used in different ways.  I think of `unit` as the type name and `()` as a way
// of creating `type unit`.
//
// the (. ) below tells bucklescript not to curry this function.
///

/**
 * Returns a random number between `min` and `max`.
 */
[@bs.module "big-integer"]
external randBetween:
  (
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    [@bs.unwrap] [ | `String(string) | `Int(int) | `BigInt(t)],
    ~func: (. unit) => float=?,
    unit
  ) =>
  t =
  "randBetween";

// ============Override Methods================

/**
 * Converts a bigInt to a native Javascript number.
 */
[@bs.send]
external valueOf: t => int = "valueOf";

/**
 * Internal BigInteger.toString
 */
[@bs.send]
external _toString: (t, int) => string = "toString";

/**
 * Converts a bigInt to a string. There is an optional radix parameter
 * (which **defaults to 10**) that converts the number to the given radix.
 * Digits in the range `10-35` will use the letters `a-z`. Bases larger
 * than 36 are supported. If a digit is greater  * than or equal to 36,
 * it will be enclosed in angle brackets.
 */
let toString = (t, ~base=10, ()) => _toString(t, base);
