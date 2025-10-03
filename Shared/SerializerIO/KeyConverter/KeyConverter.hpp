#pragma once
//���ο� Ű���� ���鶧���� �� ���Ͽ��� ���ο� Ű���� �����ؼ� �����.
//Ű ��ȯ�� �����ϰ� �����ϴ� Define �ڵ�. Ű�� ���ø� Ư��ȭ�� �� �־ �Ȱ��� ������ ��� �ؾ� �ϱ� ����.
#ifndef DECLARE_KEY_CONVERTER
#define DECLARE_KEY_CONVERTER(Type)                 \
	template<>                                         \
	struct KeyConverter<Type> {                        \
		static string ToKey(const Type&);     \
		static Type FromKey(const string&);   \
	};
#endif //DECLARE_KEY_CONVERTER

//Key ��ȯ ����
template<typename K>
struct KeyConverter {
	static string ToKey(const K& key) { return key; }
	static K FromKey(const string& key) { return key; }
};

//Key Ư��ȭ
DECLARE_KEY_CONVERTER(int)
DECLARE_KEY_CONVERTER(wstring)

template<typename K> string ToKeyString(const K& key) { return KeyConverter<K>::ToKey(key); }
template<typename K> K FromKeyString(const string& key) { return KeyConverter<K>::FromKey(key); }
