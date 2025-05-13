'''
Map graph data points in a file, such as:
35	1033
35	103482
35	103515
35	1050679
35	1103960
35	1103985
35	1109199
35	1112911
to continuous integers starting from 0, and save the mapping relationship:
35	0
1033	1

Usage: python map_points.py <data_file_path> <mapped_data_save_path> [<mapping_relationship_save_path>]
'''

import sys

def map_to_continuous_integers(file_path,save_path,mapping_path=None):
    # 读取文件内容
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # 提取所有点的集合
    points = set()
    for line in lines:
        point1, point2 = map(int, line.split())
        points.add(point1)
        points.add(point2)

    # 创建映射字典
    mapping = {point: idx for idx, point in enumerate(points)}

    # 替换文件中的点
    with open(save_path, 'w') as file:
        for line in lines:
            point1, point2 = map(int, line.split())
            new_line = f"{mapping[point1]}\t{mapping[point2]}\n"
            file.write(new_line)
    print(f"映射完成，数据文件已保存：{save_path}")

    # 保存映射关系
    if mapping_path:
        with open(mapping_path, 'w') as file:
            for point, idx in mapping.items():
                file.write(f"{point}\t{idx}\n")
        print(f"映射关系已保存：{mapping_path}")


if __name__ == "__main__":
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        print("用法: python map_points.py <数据文件路径> <映射后数据保存路径> [<映射关系保存路径>")
        sys.exit(1)

    file_path = sys.argv[1]
    save_path = sys.argv[2]
    mapping_path = sys.argv[3] if len(sys.argv) == 4 else None
    map_to_continuous_integers(file_path,save_path,mapping_path)

