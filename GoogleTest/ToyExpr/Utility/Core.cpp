#include "pch.h"
#include "Core/Utils/Common.h"

namespace Utility
{
	const int IMAGE_HEIGHT = 7;
	const int IMAGE_WIDTH = 7;

	vector<Rectangle> findRectangles(int image[IMAGE_HEIGHT][IMAGE_WIDTH])
	{
		vector<Rectangle> rectangles;
		bool visited[IMAGE_HEIGHT][IMAGE_WIDTH] = { false };

		auto isValid = [&](int x, int y) {
			return x >= 0 && x < IMAGE_WIDTH && y >= 0 && y < IMAGE_HEIGHT;
			};

		for (int y = 0; y < IMAGE_HEIGHT; ++y)
		{
			for (int x = 0; x < IMAGE_WIDTH; ++x)
			{
				if (!visited[y][x] && image[y][x] != 0)
				{
					// BFS 시작
					int minX = x, minY = y, maxX = x, maxY = y;
					queue<pair<int, int>> q;
					q.push({ x, y });
					visited[y][x] = true;

					while (!q.empty())
					{
						auto [cx, cy] = q.front();
						q.pop();

						minX = std::min(minX, cx);
						minY = std::min(minY, cy);
						maxX = std::max(maxX, cx);
						maxY = std::max(maxY, cy);

						// 4방향 탐색
						const int dx[] = { -1, 1, 0, 0 };
						const int dy[] = { 0, 0, -1, 1 };
						for (int d = 0; d < 4; ++d)
						{
							int nx = cx + dx[d], ny = cy + dy[d];
							if (isValid(nx, ny))
							{
								bool& curVisited = visited[ny][nx];
								if (!curVisited && image[ny][nx] != 0)
								{
									curVisited = true;
									q.push({ nx, ny });
								}
							}
						}
					}

					int width = maxX - minX + 1;
					int height = maxY - minY + 1;
					rectangles.emplace_back(minX, minY, width, height);
				}
			}
		}

		return rectangles;
	}

	TEST(Core, FindRectangles)
	{
		int image[IMAGE_HEIGHT][IMAGE_WIDTH] =
		{
			{0, 0, 0, 0, 0, 1, 2},
			{0, 1, 0, 9, 0, 1, 7},
			{0, 7, 0, 4, 0, 0, 0},
			{0, 7, 6, 1, 2, 2, 0},
			{0, 0, 0, 0, 0, 1, 3},
			{0, 1, 2, 0, 0, 7, 0},
			{1, 4, 8, 0, 0, 1, 0}
		};
		auto rectangles = findRectangles(image);
		EXPECT_EQ(rectangles[0], Rectangle(5, 0, 2, 2));
		EXPECT_EQ(rectangles[1], Rectangle(1, 1, 6, 6));
		EXPECT_EQ(rectangles[2], Rectangle(0, 5, 3, 2));
	}

	TEST(Core, MergeRectangles)
	{
		vector<Rectangle> rectangles{
			Rectangle{0, 0, 3, 3},
			Rectangle{2, 0, 2, 2},	//Rectangle{0, 0, 3, 3}와 intersect
			Rectangle{5, 0, 2, 2},
			Rectangle{1, 4, 5, 3},
			Rectangle{1, 5, 2, 2}	//Rectangle{1, 4, 5, 3}에 포함
		};
		MergeRectangles(rectangles);
		EXPECT_EQ(rectangles[0], Rectangle(0, 0, 4, 3));
		EXPECT_EQ(rectangles[1], Rectangle(5, 0, 2, 2));
		EXPECT_EQ(rectangles[2], Rectangle(1, 4, 5, 3));
	}

	TEST(Core, CycleIterator)
	{
		CycleIterator addIter(1, 7);
		for (int a = 1; a != 7; ++a)
			addIter.Increase();
		EXPECT_EQ(addIter.GetCurrent(), 1);

		CycleIterator subIter(-1, 7);
		for (int a = 1; a != 2; ++a)
			subIter.Decrease();
		EXPECT_EQ(subIter.GetCurrent(), 6);
	}
}
