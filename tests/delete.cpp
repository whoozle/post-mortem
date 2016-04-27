struct Foo
{
	virtual ~Foo() { }
	int foo;
	Foo(): foo(1) { }
};

int main()
{
	Foo * foo = new Foo;
	delete foo;
//	delete foo;
	return 0;
}
